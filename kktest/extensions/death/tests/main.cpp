#include <kktest.hpp>

using namespace kktest;

extern void checkDeathTest();
extern void matchersTest();

int main(int argc, char** argv) {
    return run(argc, argv, {
            TestCase(checkDeathTest, "ext::death::checkDeath"),
            TestCase(matchersTest, "ext::death::matchers"),
    });
}
