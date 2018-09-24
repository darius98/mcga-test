#include <cassert>

#include <vector>

#include <runtime_testing>

using namespace runtime_testing;
using namespace matcher;
using namespace std;

int main() {
    vector<int> v{1, 2, 3};
    test("", [&]() {
        expect(1 + 2 == 3);
        expectMatches(1 + 2 == 3, isTrue);
        expect(1 + 2 != 4);
        expectMatches(1 + 2 == 4, isFalse);
        expectMatches(1 + 2 == 4, isNot(isTrue));
        expectMatches(1 + 2 == 3, isNot(isFalse));

        expect(1 + 2 == 3 && 1 + 2 >= 2);
        expectMatches(1 + 2, both(isEqualTo(3), isGreaterThanOrEqualTo(2)));

        expectMatches(v, isNotEmpty);
        expectMatches(v, hasSize(3));
        expectMatches(v, hasSize(isGreaterThan(2)));
        expectMatches(v, hasSize(isGreaterThanOrEqualTo(2)));
        expectMatches(v, hasSize(isGreaterThanOrEqualTo(3)));
        expectMatches(v, hasSize(isLessThan(4)));
        expectMatches(v, hasSize(isLessThanOrEqualTo(4)));
        expectMatches(v, hasSize(isLessThanOrEqualTo(3)));

        expectMatches(v, isEqualTo(vector<int>{1, 2, 3}));
        expectMatches(v, isIdenticalTo(v));

        expectMatches(v, either(isEmpty, isNotEmpty));
    });
    return 0;
}
