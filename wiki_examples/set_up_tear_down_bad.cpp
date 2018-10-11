#include <algorithm>
#include <vector>


#include <kktest>

void testCase() {
    std::vector<int> v;

    test("After pushing back 3, 4 and 5, v has size 3", [&]() {
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        expect(v.size() == 3);
        v.clear();
    });

    test("After pushing back 3, 4 and 5, v is equal to {3, 4, 5}", [&]() {
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        expect(v == std::vector<int>{3, 4, 5});
        v.clear();
    });

    test("After pushing back 3, 4 and 5, v does not contain '6'", [&]() {
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        expect(std::find(v.begin(), v.end(), 6) == v.end());
        v.clear();
    });
}
