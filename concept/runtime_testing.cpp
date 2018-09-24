#include <vector>

#include <runtime_testing>

using namespace runtime_testing;
using namespace matcher;
using namespace std;


int main() {
    vector<int> v;

    setUp([&]() {
        v = vector<int>();
    });

    test("Vector is initially empty", [&]() {
        expect(v, isEmpty);
    });

    group("After push_back()", [&]() {
        setUp([&]() {
            v.push_back(3);
        });

        test("vector is no longer empty", [&]() {
            expect(v, isNotEmpty);
        });

        test("size is 1", [&]() {
            expect(v, hasSize(1));
        });

        test("back() returns added element", [&]() {
            expect(v.back(), isEqualTo(3));
        });

        test("A second push_back() works as expected", [&]() {
            v.push_back(4);
            expect(v, isNotEmpty);
            expect(v.back(), isEqualTo(4));
        });
    });

    return getTestReport();
}