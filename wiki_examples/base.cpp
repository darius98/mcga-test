#include <kktest>

int main() {

    test("This is a passing test!", [&]() {
        expect(1 + 2 == 3);
    });

    test("This is a failing test!", [&]() {
        expect(3 * 3 == 6);
    });

    return kktest::finalizeTesting();
}
