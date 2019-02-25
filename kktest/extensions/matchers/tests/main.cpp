#include <kktest.hpp>

using namespace kktest;

extern void comparisonMatchersTest();
extern void compositeMatchersTest();
extern void functionalMatchersTest();
extern void iterableMatchersTest();
extern void numericMatchersTest();
extern void pointerMatchersTest();
extern void stringMatchersTest();
extern void truthMatchersTest();

int main(int argc, char** argv) {
    return initAndRun(argc, argv, {
        TestCase(comparisonMatchersTest, "ext::matcher::comparison"),
        TestCase(compositeMatchersTest, "ext::matcher::composite"),
        TestCase(functionalMatchersTest, "ext::matcher::functional"),
        TestCase(iterableMatchersTest, "ext::matcher::iterable"),
        TestCase(numericMatchersTest, "ext::matcher::numeric"),
        TestCase(pointerMatchersTest, "ext::matcher::pointer"),
        TestCase(stringMatchersTest, "ext::matcher::string"),
        TestCase(truthMatchersTest, "ext::matcher::truth"),
    });
}
