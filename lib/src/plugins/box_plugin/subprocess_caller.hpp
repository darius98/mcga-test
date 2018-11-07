#ifndef KKTEST_PLUGINS_BOX_PLUGIN_SUBPROCESS_CALLER_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_SUBPROCESS_CALLER_H_

#include <string>

namespace kktest {

class SubprocessCaller {
public:
    SubprocessCaller();

    void run(const std::string& procCommand);

    bool poll();

    std::string getOutput() const;

private:

    bool available = true;
    bool outputAvailable = false;

    FILE* processFileDescriptor;
    char processOutputReadBuffer[32];
    std::string processOutput;
};

}

#endif
