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

        // on encountering the "--" argument, all arguments from that point on
        // are considered positional.
        if (arg == "--") {
            onlyPositional = true;
            continue;
        }

        // all arguments after "--" are considered positional.
        if (onlyPositional) {
            positionalArguments.push_back(arg);
            continue;
        }

        // an argument that does not start with '-' (or is equal to "-") is not
        // considered special, and therefore will be treated like either a
        // positional argument or a value filler for the last unfulfilled short
        // name argument given in the format "-XYZ".
        if (!startsWith(arg, '-') || arg == "-") {
            if (lastShortName.empty()) {
                // no unfulfilled argument given by short name, considering a
                // positional argument.
                positionalArguments.push_back(arg);
            } else {
                // `lastShortName` is an unfulfilled argument given by short
                // name in the format "-XYZ v" as "Z".
                applyValue(lastShortName, arg);
                lastShortName = "";
            }
            continue;
        }

        // if we reached this point without hitting a `continue`, now the
        // current argument is definitely a special one.

        // if we had an unfulfilled argument given by short name, we will give
        // it its implicit value since it won't be fulfilled by this argument.
        if (!lastShortName.empty()) {
            applyImplicit(lastShortName);
            lastShortName = "";
        }

        auto equalPos = arg.find('=');

        // 1. for "--X", give argument "X" its implicit value
        if (startsWith(arg, "--") && equalPos == String::npos) {
            applyImplicit(arg.substr(2));
        }

        // 2. for "--X=v", give argument "X" value "v"
        if (startsWith(arg, "--") && equalPos != String::npos) {
            applyValue(arg.substr(2, equalPos - 2), arg.substr(equalPos + 1));
        }

        // 3. for "-XYZ", give arguments "X" and "Y" their implicit values, and
        // remember "Z" as the last short name, as a construct of the form
        // "-XYZ v" is allowed, equivalent with "--X --Y --Z=v".
        if (!startsWith(arg, "--") && equalPos == String::npos) {
            for (size_t j = 1; j + 1 < arg.length(); ++j) {
                applyImplicit(arg.substr(j, 1));
            }
            lastShortName = arg.substr(arg.length() - 1, 1);
        }

        // 4. for "-XYZ=v", give arguments "X" and "Y" their implicit values and
        // argument "Z" value "v".
        if (!startsWith(arg, "--") && equalPos != String::npos) {
            for (size_t j = 1; j + 1 < equalPos; ++j) {
                applyImplicit(arg.substr(j, 1));
            }
            applyValue(arg.substr(equalPos - 1, 1), arg.substr(equalPos + 1));
        }
    }
    if (!lastShortName.empty()) {
        applyImplicit(lastShortName);
    }
    return positionalArguments;
}

void ArgumentsApiImpl::addHelpFlag() {
    helpFlag = addFlag("help", "Display this help menu.", "h");
}

void ArgumentsApiImpl::checkHelpFlag() {
    if (helpFlag != nullptr && helpFlag->get()) {
        cout << help << "\n";
        exit(0);
    }
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
