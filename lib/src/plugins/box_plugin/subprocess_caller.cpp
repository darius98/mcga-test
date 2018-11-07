#include <stdexcept>

#include "subprocess_caller.hpp"

using namespace std;

namespace kktest {

SubprocessCaller::SubprocessCaller() {}

void SubprocessCaller::run(const string& procCommand) {
    if (!available) {
        throw runtime_error("Trying to run in un-available box!");
    }
    outputAvailable = false;
    available = false;

    processOutput.clear();

    processFileDescriptor = popen(procCommand.c_str(), "r");
    if (!processFileDescriptor) {
        throw runtime_error("popen() failed!");
    }
}

bool SubprocessCaller::poll() {
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

string SubprocessCaller::getOutput() const {
    if (!outputAvailable) {
        throw runtime_error("Asked box for run stats, "
                            "but run stats are not available!");
    }
    return processOutput;
}

}
