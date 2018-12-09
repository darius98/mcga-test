#include <iostream>
#include <stdexcept>

#include "arguments_api_impl.hpp"
#include "argument_impl.hpp"
#include "flag_impl.hpp"
#include "int_argument_impl.hpp"

using kktest::strutil::startsWith;
using std::cout;
using std::move;
using std::runtime_error;
using std::to_string;
using std::vector;

namespace kktest {
namespace arguments {

ArgumentsApiImpl::ArgumentsApiImpl(String helpPrefix): help(move(helpPrefix)) {}

ArgumentsApiImpl::~ArgumentsApiImpl() {
    for (CommandLineSpec* spec : commandLineSpecs) {
        delete spec;
    }
}

Argument* ArgumentsApiImpl::addArgument(const String& name,
                                        const String& helpText,
                                        const String& shortName,
                                        const String& defaultValue,
                                        const String& implicitValue) {
    checkNameAvailability(name, shortName);
    auto spec = new ArgumentImpl(defaultValue, implicitValue);
    addSpec(spec, name, helpText, shortName, defaultValue, implicitValue);
    return spec;
}

IntArgument* ArgumentsApiImpl::addIntArgument(const String& name,
                                              const String& helpText,
                                              const String& shortName,
                                              int defaultValue,
                                              int implicitValue) {
    checkNameAvailability(name, shortName);
    auto spec = new IntArgumentImpl(defaultValue, implicitValue);
    addSpec(spec,
            name,
            helpText,
            shortName,
            to_string(defaultValue),
            to_string(implicitValue));
    return spec;
}

Flag* ArgumentsApiImpl::addFlag(const String& name,
                                const String& helpText,
                                const String& shortName) {
    checkNameAvailability(name, shortName);
    auto spec = new FlagImpl();
    addSpec(spec, name, helpText, shortName, "", "");
    return spec;
}

vector<String> ArgumentsApiImpl::interpret(int argc, char** argv) {
    for (CommandLineSpec* spec : commandLineSpecs) {
        spec->setDefault();
    }
    vector<String> positionalArguments;
    String lastShortName;
    bool onlyPositional = false;
    for (int i = 1; i < argc; ++i) {
        String arg(argv[i]);
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
            if (equalPos == String::npos) {
                // is of the form --X
                applyImplicit(arg.substr(2));
            } else {
                // is of the form --X=v
                applyValue(arg.substr(2, equalPos - 2),
                           arg.substr(equalPos + 1));
            }
        } else {
            auto equalPos = arg.find('=');
            if (equalPos == String::npos) {
                // is of the form -XYZ
                for (size_t j = 1; j + 1 < arg.length(); ++j) {
                    applyImplicit(arg.substr(j, 1));
                }
                lastShortName = arg.substr(arg.length() - 1, 1);
            } else {
                // is of the form -XYZ=v
                for (size_t j = 1; j + 1 < equalPos; ++j) {
                    applyImplicit(arg.substr(j, 1));
                }
                applyValue(arg.substr(equalPos - 1, 1),
                           arg.substr(equalPos + 1));
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

void ArgumentsApiImpl::addHelpFlag() {
    helpFlag = addFlag("help", "Display this help menu.", "h");
}

void ArgumentsApiImpl::addSpec(CommandLineSpec* spec,
                               const String& name,
                               const String& helpText,
                               const String& shortName,
                               const String& defaultValue,
                               const String& implicitValue) {
    String helpLine = "\n\t--" + name;
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
    help += helpLine;
    commandLineSpecs.push_back(spec);
    reservedNames.insert(name);
    specsByCommandLineString[name] = spec;
    if (!shortName.empty()) {
        reservedNames.insert(shortName);
        specsByCommandLineString[shortName] = spec;
    }
}

void ArgumentsApiImpl::checkNameAvailability(const String& name,
                                             const String& shortName) const {
    if (reservedNames.count(name) != 0) {
        throw runtime_error(
            "Argument tried to register " + name + " as a command-line name, "
            "but a different argument already has it as a name.");
    }
    if (!shortName.empty() && reservedNames.count(shortName) != 0) {
        throw runtime_error(
            "Argument tried to register " + shortName + " as a command-line "
            "short name, but a different argument already has it as a short "
            "name.");
    }
    if (shortName.size() > 1) {
        throw runtime_error("Argument short name should always have length 1.");
    }
}

void ArgumentsApiImpl::applyValue(const String& commandLineString,
                                  const String& value) {
    auto specIterator = specsByCommandLineString.find(commandLineString);
    if (specIterator != specsByCommandLineString.end()) {
        specIterator->second->setValue(value);
    }
}

void ArgumentsApiImpl::applyImplicit(const String& commandLineString) {
    auto specIterator = specsByCommandLineString.find(commandLineString);
    if (specIterator != specsByCommandLineString.end()) {
        specIterator->second->setImplicit();
    }
}

}  // namespace arguments
}  // namespace kktest
