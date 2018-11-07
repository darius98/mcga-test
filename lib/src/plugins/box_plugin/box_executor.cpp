#include <unistd.h>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

BoxExecutor::BoxExecutor(int numBoxes) {
    for (int i = 0; i < numBoxes; ++ i) {
        containers.emplace_back();
    }
}

void BoxExecutor::execute(Test* test, Executable func, Executable after) {
    TestContainer& container = findEmptyContainer();
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
        // TODO: Close read end of the pipe
        run(test, func);
        string output = test->toJSON().stringify();
        int bytesToWrite = output.size() + 1;
        const char* bytes = output.c_str();
        int numBytesWritten = 0;
        while (bytesToWrite > 0) {
            int blockSize = write(fd[1], bytes + numBytesWritten, bytesToWrite);
            if (blockSize < 0) {
                perror("write");
                exit(errno);
            }
            bytesToWrite -= blockSize;
            numBytesWritten += blockSize;
        }
        exit(0);
    }
    close(fd[1]);
    container.fill(test, fd[0], after);
}

TestContainer& BoxExecutor::findEmptyContainer() {
    while(true) {
        for (TestContainer &container : containers) {
            if (tryFinalizeContainer(container)) {
                return container;
            }
        }
    }
}

bool BoxExecutor::tryFinalizeContainer(TestContainer& container) {
    Test* test = container.getTest();
    if (container.tryFinalize()) {
        if (test != nullptr) {
            test->loadFromJSON(JSON::parse(container.getOutput()));
            container.executeAfter();
        }
        return true;
    }
    return false;
}

void BoxExecutor::finalize() {
    bool allFinalized = false;
    while (!allFinalized) {
        allFinalized = true;
        for (TestContainer& container: containers) {
            if (!tryFinalizeContainer(container)) {
                allFinalized = false;
            }
        }
    }
}

}
