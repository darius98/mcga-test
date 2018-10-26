#include "box.hpp"

using namespace autojson;
using namespace std;

namespace kktest {

Box::Box(string _boxId, string _binaryPath):
    boxId(move(_boxId)),
    binaryPath(move(_binaryPath)),
    copiedBinary(false),
    available(true) {}

void Box::run(const string& runArgs) {
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

bool Box::poll() {
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

JSON Box::getRunStats() const {
    if (!runStatsAvailable) {
        throw runtime_error("Asked box for run stats, "
                            "but run stats are not available!");
    }
    return runStats;
}

string Box::getBoxDirPath() const {
    return "/tmp/box/" + boxId + "/box/";
}

string Box::getMetaFilePath() const {
    return getBoxDirPath() + "meta.json";
}

string Box::getBoxedBinaryPath() const {
    return getBoxDirPath() + "binary";
}

}
