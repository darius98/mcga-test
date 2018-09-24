#include <cassert>

#include <vector>

#include <runtime_testing>

using namespace runtime_testing;
using namespace matcher;
using namespace std;

int main() {
    vector<int> v{1, 2, 3};

    expect(1 + 2 == 3, isTrue);
    expect(1 + 2 == 4, isFalse);

    expect(1 + 2 == 3, isNot(isFalse));
    expect(1 + 2 == 4, isNot(isTrue));
    expect(1 + 2, both(isEqualTo(3), isGreaterThanOrEqualTo(2)));

    expect(v, isNotEmpty);
    expect(v, hasSize(3));
    expect(v, hasSize(isGreaterThan(2)));
    expect(v, hasSize(isGreaterThanOrEqualTo(2)));
    expect(v, hasSize(isGreaterThanOrEqualTo(3)));
    expect(v, hasSize(isLessThan(4)));
    expect(v, hasSize(isLessThanOrEqualTo(4)));
    expect(v, hasSize(isLessThanOrEqualTo(3)));

    expect(v, isEqualTo(vector<int>{1, 2, 3}));
    expect(v, isIdenticalTo(v));

    expect(v, either(isEmpty, isNotEmpty));

    try {
        expect(v, hasSize(isGreaterThan(5)));
    } catch(const ExpectationFailed& failure) {
        assert(
            failure.getMessage() ==
            "expected container of size greater than '5', got '3': '[1, 2, 3]'"
        );
    }

    return 0;
}
