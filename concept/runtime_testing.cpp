#include <fstream>
#include <vector>

#include <runtime_testing>

using namespace runtime_testing;
using namespace matcher;
using namespace std;


int main() {
    initializeTestingDriver(cout);

    vector<int> v;

    setUp([&]() {
        v = vector<int>();
    });

    test("Vector is initially empty", [&]() {
        expect(v.empty());
        expectMatches(v, isEmpty);
    });

    group("After push_back()", [&]() {
        setUp([&]() {
            v.push_back(3);
        });

        test("vector is no longer empty", [&]() {
            expect(!v.empty());
            expectMatches(v, isNotEmpty);
        });

        test("size is 1", [&]() {
            expect(v.size() == 1);
            expectMatches(v, hasSize(1));
        });

        test("back() returns added element", [&]() {
            expect(v.back() == 3);
            expectMatches(v.back(), isEqualTo(5)); // This will fail!
        });

        test("A second push_back() works as expected", [&]() {
            v.push_back(4);

            expectMatches(v, eachElement(isGreaterThan(2)));
            expectMatches(v, eachElement(2));

            expect(v.empty()); // This will fail!
            expectMatches(v, isNotEmpty);

            expect(v.back() == 4);
            expectMatches(v.back(), isEqualTo(4));
        });
    });

    ofstream f("report.json");
    int status = getTestSuiteReport(f);
    destroyTestingDriver();
    return status;
}
