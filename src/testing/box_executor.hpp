#ifndef KKTEST_TESTING_BOX_EXECUTOR_H_
#define KKTEST_TESTING_BOX_EXECUTOR_H_

#include "executor.hpp"


namespace kktest {

class BoxWrapper {
public:
    BoxWrapper(std::string _boxId, std::string _binaryPath);

    void run(int testIndex);

    bool poll();

    std::pair<std::string, autojson::JSON> getRunStats() const;
private:
    std::string boxId;
    std::string binaryPath;
    bool copiedBinary;
    bool available;

    FILE* processFileDescriptor;
    char processOutputReadBuffer[32];

    bool runStatsAvailable = false;
    std::string processOutput;
    autojson::JSON runStats;
};

class BoxExecutor: public Executor {
public:
    explicit BoxExecutor(int testIndexToRun, std::string binaryPath);

    ~BoxExecutor();

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

private:
    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func,
                 int testIndex) override;

    Box* box;
};

}

#endif
