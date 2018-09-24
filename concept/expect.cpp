#include <vector>

#include <runtime_testing>

using namespace testing;
using namespace matcher;
using namespace std;

int main() {
    vector<int> v{1, 2, 3};
    expect(v, isNotEmpty);
    expect(v, hasSize(3));
    expect(v, hasSize(greaterThan(2)));
    expect(v, hasSize(greaterThanOrEqualTo(2)));
    expect(v, hasSize(greaterThanOrEqualTo(3)));
    expect(v, hasSize(lessThan(4)));
    expect(v, hasSize(lessThanOrEqualTo(4)));
    expect(v, hasSize(lessThanOrEqualTo(3)));

}
