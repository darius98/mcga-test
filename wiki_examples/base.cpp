#include <kktest>

int main(int argc, char** argv) {
    initializeTesting(argc, argv);

    test("This is a passing test!", [&]() {
        expect(1 + 2 == 3);
    });

    test("This is a failing test!", [&]() {
        expect(3 * 3 == 6);
    });

    return finalizeTesting();
}
