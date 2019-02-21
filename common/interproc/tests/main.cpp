#include <kktest.hpp>

using namespace kktest;

extern void interprocMessageTest();
extern void interprocPipeTest();

int main(int argc, char** argv) {
    return run(argc, argv, {
            TestCase(interprocMessageTest, "Interproc::Message"),
            TestCase(interprocPipeTest, "Interproc::Pipe"),
    });
}
