#include <cassert>

#include <vector>

#include <runtime_testing>

using namespace testing;
using namespace matcher;
using namespace std;

int main() {
    vector<int> v{1, 2, 3};

    expect(1 + 2 == 3, isTrue);
    expect(1 + 2 == 4, isFalse);

    expect(1 + 2 == 3, isNot(isFalse));
    expect(1 + 2 == 4, isNot(isTrue));
    expect(1 + 2, both(equal(3), greaterThanOrEqualTo(2)));

    expect(v, isNotEmpty);
    expect(v, hasSize(3));
    expect(v, hasSize(greaterThan(2)));
    expect(v, hasSize(greaterThanOrEqualTo(2)));
    expect(v, hasSize(greaterThanOrEqualTo(3)));
    expect(v, hasSize(lessThan(4)));
    expect(v, hasSize(lessThanOrEqualTo(4)));
    expect(v, hasSize(lessThanOrEqualTo(3)));

    expect(v, equal(vector<int>{1, 2, 3}));
    expect(v, identical(v));

    expect(v, either(isEmpty, isNotEmpty));

    try {
        expect(v, hasSize(greaterThanOrEqualTo(5)));
    } catch(const ExpectationFailed& failure) {
        assert(
            failure.getMessage() ==
            "expected container of size greater than or equal to '5', got '3': "
            "'[1, 2, 3]'"
        );
    }

    return 0;
}
