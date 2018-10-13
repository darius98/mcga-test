#include "box_wrapper.hpp"

using namespace autojson;
using namespace std;

namespace kktest {

BoxWrapper::BoxWrapper(string _boxId, string _binaryPath):
    boxId(move(_boxId)),
    binaryPath(move(_binaryPath)),
    copiedBinary(false),
    available(true) {}

void BoxWrapper::run(const string& runArgs) {
    if (!available) {
        throw runtime_error("Trying to run in un-available box!");
    }
    runStatsAvailable = false;
    available = false;

    if (!copiedBinary) {
        string copyCommand =   "mkdir -p " + getBoxDirPath()
                             + " && "
                             + "cp " + binaryPath + " " + getBoxedBinaryPath();
        system(copyCommand.c_str());
        copiedBinary = true;
    }

    processOutput.clear();

    string boxedProcess = "box --run --meta=" + getMetaFilePath()
                          + " --box-id=" + boxId + " -- ./binary " + runArgs;
    processFileDescriptor = popen(boxedProcess.c_str(), "r");
    if (!processFileDescriptor) {
        throw runtime_error("popen() failed!");
    }
}

bool BoxWrapper::poll() {
    if (available) {
        return true;
    }

    if (fgets(processOutputReadBuffer, 32, processFileDescriptor) != nullptr) {
        processOutput += processOutputReadBuffer;
    }
    if (feof(processFileDescriptor)) {
        pclose(processFileDescriptor);
        runStats = JSON::readFromFile(getMetaFilePath());
        runStats["processOutput"] = processOutput;
        runStatsAvailable = true;
        available = true;
    }
    return available;
}

JSON BoxWrapper::getRunStats() const {
    if (!runStatsAvailable) {
        throw runtime_error("Asked box for run stats, "
                            "but run stats are not available!");
    }
    return runStats;
}

string BoxWrapper::getBoxDirPath() const {
    return "/tmp/box/" + boxId + "/box/";
}

string BoxWrapper::getMetaFilePath() const {
    return getBoxDirPath() + "meta.json";
}

string BoxWrapper::getBoxedBinaryPath() const {
    return getBoxDirPath() + "binary";
}

}
