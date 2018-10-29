#ifndef KKTEST_MODULES_BOX_MODULE_BOX_H_
#define KKTEST_MODULES_BOX_MODULE_BOX_H_

#include <string>

#include <JSON>

namespace kktest {

class Box {
public:
    Box(std::string _boxId, std::string _binaryPath);

    void run(const std::string& runArgs);

    bool poll();

    autojson::JSON getRunStats() const;

private:
    std::string getBoxDirPath() const;

    std::string getMetaFilePath() const;

    std::string getBoxedBinaryPath() const;

    std::string boxId;
    std::string binaryPath;
    bool copiedBinary;

    bool available = true;
    bool runStatsAvailable = false;

    FILE* processFileDescriptor;
    char processOutputReadBuffer[32];
    std::string processOutput;
    autojson::JSON runStats;
};

}

#endif
