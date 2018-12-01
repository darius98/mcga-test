#include <iostream>

#include <strutil>

#include "arguments_api_impl.hpp"

using namespace std;
using namespace strutil;

namespace kktest {

Flag::Flag(): value(false) {}

bool Flag::get() const {
    return value;
}

void Flag::setValue(bool _value) {
    value = _value;
}

Argument::Argument(string _value): value(move(_value)) {}

std::string Argument::get() const {
    return value;
}

void Argument::setValue(const std::string& _value) {
    value = _value;
}

ArgumentsApi* ArgumentsApi::create(const string& helpPrefix) {
    auto api = new ArgumentsApiImpl(helpPrefix);
    api->addHelpFlag();
    return api;
}

ArgumentSpec::ArgumentSpec(const string& defaultValue, const string& _implicitValue):
        implicitValue(_implicitValue), argument(new Argument(defaultValue)) {}

ArgumentSpec::~ArgumentSpec() {
    delete argument;
}

Argument* ArgumentSpec::getArgument() {
    return argument;
}

void ArgumentSpec::setImplicit() {
    argument->setValue(implicitValue);
}

void ArgumentSpec::setValue(const string& value) {
    argument->setValue(value);
}

FlagSpec::FlagSpec(): flag(new Flag()) {}

FlagSpec::~FlagSpec() {
    delete flag;
}

Flag* FlagSpec::getFlag() {
    return flag;
}

void FlagSpec::setValue(bool value) {
    flag->setValue(value);
}

ArgumentsApiImpl::ArgumentsApiImpl(string helpPrefix): help(move(helpPrefix)) {}

ArgumentsApiImpl::~ArgumentsApiImpl() {
    for (FlagSpec* flagSpec: flagsSpec) {
        delete flagSpec;
    }
    for (ArgumentSpec* argumentSpec: argumentsSpec) {
        delete argumentSpec;
    }
}

Argument* ArgumentsApiImpl::addArgument(const string& name,
                                        const string& helpText,
                                        const string& shortName,
                                        const string& defaultValue,
                                        const string& implicitValue) {
    if (reservedNames.count(name) != 0) {
        throw runtime_error("Argument tried to register " + name + " as a command-line name, but "
                            "a different argument already has it as a name.");
    }
    if (!shortName.empty() && reservedNames.count(shortName) != 0) {
        throw runtime_error("Argument tried to register " + shortName + " as a command-line short "
                            "name, but a different argument already has it as a short name.");
    }
    if (shortName.size() > 1) {
        throw runtime_error("Argument short name should always have length 1.");
    }
    auto spec = new ArgumentSpec(defaultValue, implicitValue);
    argumentsSpec.push_back(spec);
    reservedNames.insert(name);
    argumentSpecsByCommandLineString[name] = spec;
    if (!shortName.empty()) {
        reservedNames.insert(shortName);
        argumentSpecsByCommandLineString[shortName] = spec;
    }
    help += getHelpSection(name, helpText, shortName, defaultValue, implicitValue);
    return spec->getArgument();
}

Flag* ArgumentsApiImpl::addFlag(const string& name,
                                const string& helpText,
                                const string& shortName) {
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
    auto spec = new FlagSpec();
    flagsSpec.push_back(spec);
    reservedNames.insert(name);
    flagSpecsByCommandLineString[name] = spec;
    if (!shortName.empty()) {
        reservedNames.insert(shortName);
        flagSpecsByCommandLineString[shortName] = spec;
    }
    help += getHelpSection(name, helpText, shortName, "", "");
    return spec->getFlag();
}

void ArgumentsApiImpl::addHelpFlag() {
    helpFlag = addFlag("help", "Display this help menu.", "h");
}

string ArgumentsApiImpl::getHelpSection(const string& name,
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

vector<string> ArgumentsApiImpl::interpret(int argc, char** argv) {
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

void ArgumentsApiImpl::applyValue(const string& commandLineString, const string& value) {
    auto argSpecIterator = argumentSpecsByCommandLineString.find(commandLineString);
    if (argSpecIterator != argumentSpecsByCommandLineString.end()) {
        argSpecIterator->second->setValue(value);
    }
    auto flagSpecIterator = flagSpecsByCommandLineString.find(commandLineString);
    if (flagSpecIterator != flagSpecsByCommandLineString.end()) {
        flagSpecIterator->second->setValue(
            toLower(value) == "true" || value == "1" || value == "enabled"
        );
    }
}

void ArgumentsApiImpl::applyImplicit(const std::string& commandLineString) {
    auto argSpecIterator = argumentSpecsByCommandLineString.find(commandLineString);
    if (argSpecIterator != argumentSpecsByCommandLineString.end()) {
        argSpecIterator->second->setImplicit();
    }
    auto flagSpecIterator = flagSpecsByCommandLineString.find(commandLineString);
    if (flagSpecIterator != flagSpecsByCommandLineString.end()) {
        flagSpecIterator->second->setValue(true);
    }
}

}
