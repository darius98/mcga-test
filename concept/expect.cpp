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
        expect(1 + 2 != 4);

        expectMatches(1 + 2 == 3, isTrue);
        expectMatches(1 + 2 == 4, isFalse);
        expectMatches(1 + 2 == 4, isNot(isTrue));
        expectMatches(1 + 2 == 3, isNot(isFalse));
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

        expectMatches(v, either(eachElement(isEqualTo(2)), isNotEmpty));

        expectMatches(nullptr, isNull);
        expectMatches(new int(), isNotNull);

        expectMatches(3.5, isAlmostEqualTo(3.49999999));
        expectMatches(3.5, isAlmostEqualTo(3.4, 0.2));
        expectMatches(3.5, isPositive);
        expectMatches(3.5, isNot(isNegative));
        expectMatches(-3.5, isNegative);
        expectMatches(-3.5, isNot(isPositive));
        expectMatches(3, isOdd);
        expectMatches(3u, isOdd);
        expectMatches(3L, isOdd);
        expectMatches(3LL, isOdd);
        expectMatches(4, isEven);
        expectMatches(4u, isEven);
        expectMatches(4L, isEven);
        expectMatches(4LL, isEven);
        expectMatches(0, isZero);
        expectMatches(0u, isZero);
        expectMatches(0L, isZero);
        expectMatches(0LL, isZero);
        expectMatches(v, eachElement(isPositive));
    });
    return 0;
}
