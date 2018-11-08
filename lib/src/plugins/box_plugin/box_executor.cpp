#include <unistd.h>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;


namespace {

void writeStringToPipe(const std::string& output, int pipeFD) {
    size_t bytesToWrite = output.size() + 1;
    const char* bytes = output.c_str();
    int written = 0;
    while (bytesToWrite > 0) {
        ssize_t blockSize = write(pipeFD, bytes + written, bytesToWrite);
        if (blockSize < 0) {
            perror("write");
            exit(errno);
        }
        bytesToWrite -= blockSize;
        written += blockSize;
    }
}

}


namespace kktest {

BoxExecutor::BoxExecutor(std::size_t _maxNumContainers):
        maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func, Executable after) {
    ensureFreeContainers(1);
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(errno);
    }
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(errno);
    }
    if (pid == 0) { // child
        close(fd[0]);
        run(test, func);
        writeStringToPipe(test->toJSON().stringify(), fd[1]);
        exit(0);
    }
    close(fd[1]);
    openContainers.insert(new TestContainer(test, fd[0], pid, after));
}

void BoxExecutor::finalize() {
    ensureFreeContainers(maxNumContainers);
}

void BoxExecutor::ensureFreeContainers(size_t numContainers) {
    while (openContainers.size() > maxNumContainers - numContainers) {
        auto it = openContainers.begin();
        while (it != openContainers.end()) {
            if ((*it)->isTestFinished()) {
                delete (*it);
                it = openContainers.erase(it);
            } else {
                ++ it;
            }
        }
    }
}

}
