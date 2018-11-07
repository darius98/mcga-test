#include <stdexcept>

#include "box.hpp"

using namespace std;

namespace kktest {

Box::Box(string _binaryPath): binaryPath(move(_binaryPath)), available(true) {}

void Box::run(const string& runArgs) {
    if (!available) {
        throw runtime_error("Trying to run in un-available box!");
    }
    outputAvailable = false;
    available = false;

    processOutput.clear();

    string procCommand = binaryPath + " " + runArgs;
    processFileDescriptor = popen(procCommand.c_str(), "r");
    if (!processFileDescriptor) {
        throw runtime_error("popen() failed!");
    }
}

bool Box::poll() {
    if (available) {
        return true;
    }

    if (fgets(processOutputReadBuffer, 32, processFileDescriptor) != nullptr) {
        processOutput += processOutputReadBuffer;
    }
    if (feof(processFileDescriptor)) {
        pclose(processFileDescriptor);
        outputAvailable = true;
        available = true;
    }
    return available;
}

string Box::getOutput() const {
    if (!outputAvailable) {
        throw runtime_error("Asked box for run stats, "
                            "but run stats are not available!");
    }
    return processOutput;
}

}
