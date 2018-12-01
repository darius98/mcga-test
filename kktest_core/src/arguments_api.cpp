#include <iostream>
#include <map>
#include <set>

#include <strutil>

#include <kktest_impl/arguments_api.hpp>

using std::cout;
using std::map;
using std::runtime_error;
using std::set;
using std::string;
using std::vector;
using strutil::startsWith;
using strutil::toLower;

namespace kktest {

class CommandLineSpecImpl {
public:
    virtual ~CommandLineSpecImpl() = default;

    virtual void setDefault() = 0;

    virtual void setImplicit() = 0;

    virtual void setValue(const string& _value) = 0;
};

class FlagImpl: public Flag, public CommandLineSpecImpl {
public:
    ~FlagImpl() override = default;

    bool get() const override {
        return value;
    }

    void setDefault() override {
        value = false;
    }

    void setImplicit() override {
        value = true;
    }

    void setValue(const string& _value) override {
        value = toLower(_value) == "true" || _value == "1" || toLower(_value) == "enabled";
    }

private:
    bool value = false;
};

class ArgumentImpl: public Argument, public CommandLineSpecImpl {
public:
    ArgumentImpl(const string& _defaultValue, string _implicitValue):
            value(_defaultValue),
            defaultValue(_defaultValue),
            implicitValue(move(_implicitValue)) {}

    ~ArgumentImpl() override = default;

    string get() const override {
        return value;
    }

    void setDefault() override {
        value = defaultValue;
    }

    void setImplicit() override {
        value = implicitValue;
    }

    void setValue(const string& _value) override {
        value = _value;
    }

private:
    string value;
    string defaultValue;
    string implicitValue;
};

class ArgumentsApiImpl: public ArgumentsApi {
public:
    explicit ArgumentsApiImpl(string helpPrefix): help(move(helpPrefix)) {}

    ~ArgumentsApiImpl() override {
        for (CommandLineSpecImpl* spec: commandLineSpecs) {
            delete spec;
        }
    }

    Argument* addArgument(const string& name,
                          const string& helpText,
                          const string& shortName,
                          const string& defaultValue,
                          const string& implicitValue) override {
        if (reservedNames.count(name) != 0) {
            throw runtime_error(
                "Argument tried to register " + name + " as a command-line name, "
                "but a different argument already has it as a name."
            );
        }
        if (!shortName.empty() && reservedNames.count(shortName) != 0) {
            throw runtime_error(
                "Argument tried to register " + shortName + " as a command-line "
                "short name, but a different argument already has it as a short name."
            );
        }
        if (shortName.size() > 1) {
            throw runtime_error("Argument short name should always have length 1.");
        }
        auto spec = new ArgumentImpl(defaultValue, implicitValue);
        commandLineSpecs.push_back(spec);
        reservedNames.insert(name);
        specsByCommandLineString[name] = spec;
        if (!shortName.empty()) {
            reservedNames.insert(shortName);
            specsByCommandLineString[shortName] = spec;
        }
        help += getHelpSection(name, helpText, shortName, defaultValue, implicitValue);
        return spec;
    }

    Flag* addFlag(const string& name, const string& helpText, const string& shortName) override {
        if (reservedNames.count(name) != 0) {
            throw runtime_error("Flag tried to register " + name + " as a command-line name, but "
                                "a different argument already has it as a name.");
        }
        if (!shortName.empty() && reservedNames.count(shortName) != 0) {
            throw runtime_error("Flag tried to register " + shortName + " as a command-line short "
                                "name, but a different argument already has it as a short name.");
        }
        if (shortName.size() > 1) {
            throw runtime_error("Argument short name should always have length 1.");
        }
        auto spec = new FlagImpl();
        commandLineSpecs.push_back(spec);
        reservedNames.insert(name);
        specsByCommandLineString[name] = spec;
        if (!shortName.empty()) {
            reservedNames.insert(shortName);
            specsByCommandLineString[shortName] = spec;
        }
        help += getHelpSection(name, helpText, shortName, "", "");
        return spec;
    }

    vector<string> interpret(int argc, char** argv) override {
        for (CommandLineSpecImpl* spec: commandLineSpecs) {
            spec->setDefault();
        }
        vector<string> positionalArguments;
        string lastShortName;
        bool onlyPositional = false;
        for (int i = 1; i < argc; ++ i) {
            string arg(argv[i]);
            if (arg == "--") {
                onlyPositional = true;
                continue;
            }
            if (onlyPositional) {
                positionalArguments.push_back(arg);
                continue;
            }
            if (!startsWith(arg, '-') || arg == "-") {
                if (lastShortName.empty()) {
                    positionalArguments.push_back(arg);
                } else {
                    applyValue(lastShortName, arg);
                    lastShortName = "";
                }
                continue;
            }

            if (!lastShortName.empty()) {
                applyImplicit(lastShortName);
                lastShortName = "";
            }

            // is of the form "-XYZ" or "--X" or "-XYZ=v" or "--X=v"
            if (startsWith(arg, "--")) {
                auto equalPos = arg.find('=');
                if (equalPos == string::npos) {
                    // is of the form --X
                    applyImplicit(arg.substr(2));
                } else {
                    // is of the form --X=v
                    applyValue(arg.substr(2, equalPos - 2), arg.substr(equalPos + 1));
                }
            } else {
                auto equalPos = arg.find('=');
                if (equalPos == string::npos) {
                    // is of the form -XYZ
                    for (size_t j = 1; j + 1 < arg.length(); ++ j) {
                        applyImplicit(arg.substr(j, 1));
                    }
                    lastShortName = arg.substr(arg.length() - 1, 1);
                } else {
                    // is of the form -XYZ=v
                    for (size_t j = 1; j + 1 < equalPos; ++ j) {
                        applyImplicit(arg.substr(j, 1));
                    }
                    applyValue(arg.substr(equalPos - 1, 1), arg.substr(equalPos + 1));
                }
            }
        }
        if (!lastShortName.empty()) {
            applyImplicit(lastShortName);
        }
        if (helpFlag != nullptr && helpFlag->get()) {
            cout << help << "\n";
            exit(0);
        }
        return positionalArguments;
    }

    void addHelpFlag() {
        helpFlag = addFlag("help", "Display this help menu.", "h");
    }

private:
    string getHelpSection(const string& name,
                          const string& helpText,
                          const string& shortName,
                          const string& defaultValue,
                          const string& implicitValue) {
        string helpLine = "\n\t--" + name;
        if (!shortName.empty()) {
            helpLine += ",-" + shortName;
        }
        helpLine += "\t" + helpText;
        if (!defaultValue.empty() || !implicitValue.empty()) {
            helpLine += "\n\t\t";
            if (!defaultValue.empty()) {
                helpLine += "Default: " + defaultValue;
                if (!implicitValue.empty()) {
                    helpLine += ", ";
                }
            }
            if (!implicitValue.empty()) {
                helpLine += "Implicit: " + implicitValue;
            }
        }
        helpLine += "\n";
        return helpLine;
    }

    void applyValue(const string& commandLineString, const string& value) {
        auto specIterator = specsByCommandLineString.find(commandLineString);
        if (specIterator != specsByCommandLineString.end()) {
            specIterator->second->setValue(value);
        }
    }

    void applyImplicit(const string& commandLineString) {
        auto specIterator = specsByCommandLineString.find(commandLineString);
        if (specIterator != specsByCommandLineString.end()) {
            specIterator->second->setImplicit();
        }
    }

    Flag* helpFlag = nullptr;

    vector<CommandLineSpecImpl*> commandLineSpecs;
    map<string, CommandLineSpecImpl*> specsByCommandLineString;

    string help;
    set<string> reservedNames;
};

ArgumentsApi* ArgumentsApi::create(const string& helpPrefix) {
    auto api = new ArgumentsApiImpl(helpPrefix);
    api->addHelpFlag();
    return api;
}

}
