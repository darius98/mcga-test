#include <algorithm>
#include <vector>

#include <kktest>

void kkTestCase() {
    std::vector<int> v;

    kkTest("After pushing back 3, 4 and 5, v has size 3", [&]() {
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        kkExpect(v.size() == 3);
        v.clear();
    });

    kkTest("After pushing back 3, 4 and 5, v is equal to {3, 4, 5}", [&]() {
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        kkExpect(v == std::vector<int>{3, 4, 5});
        v.clear();
    });

    kkTest("After pushing back 3, 4 and 5, v does not contain '6'", [&]() {
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        kkExpect(std::find(v.begin(), v.end(), 6) == v.end());
        v.clear();
    });
}
