#ifndef KKTEST_PLUGINS_BOX_PLUGIN_BOX_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_BOX_H_

#include <string>

namespace kktest {

class Box {
public:
    Box(std::string _binaryPath);

    void run(const std::string& runArgs);

    bool poll();

    std::string getOutput() const;

private:

    std::string binaryPath;

    bool available = true;
    bool outputAvailable = false;

    FILE* processFileDescriptor;
    char processOutputReadBuffer[32];
    std::string processOutput;
};

}

#endif
