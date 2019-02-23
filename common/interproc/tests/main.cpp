#include <kktest.hpp>

using namespace kktest;

extern void interprocMessageTest();
extern void interprocPipeTest();
extern void interprocSubprocessTest();
extern void interprocWorkerSubprocessTest();

int main(int argc, char** argv) {
    return run(argc, argv, {
        TestCase(interprocMessageTest, "Interproc::Message"),
        TestCase(interprocPipeTest, "Interproc::Pipe"),
        TestCase(interprocSubprocessTest, "Interproc::Subprocess"),
        TestCase(interprocWorkerSubprocessTest, "Interproc::WorkerSubprocess"),
    });
}
