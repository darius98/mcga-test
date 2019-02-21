#include <kktest.hpp>

using namespace kktest;

extern void utilsTimeTest();

int main(int argc, char** argv) {
    return run(argc, argv, {
            TestCase(utilsTimeTest, "Utils::Time")
    });
}
