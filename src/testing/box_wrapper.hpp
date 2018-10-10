#ifndef KKTEST_TESTING_BOX_WRAPPER_H_
#define KKTEST_TESTING_BOX_WRAPPER_H_

#include <string>

#include <JSON>

namespace kktest {

class BoxWrapper {
public:
    BoxWrapper(std::string _boxId, std::string _binaryPath);

    void run(const std::string& runArgs);

    bool poll();

    std::pair<std::string, autojson::JSON> getRunStats() const;

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
