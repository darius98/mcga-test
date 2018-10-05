#include <algorithm>
#include <kktest>

int main(int argc, char** argv) {
    kktest::initializeTestingDriver(argc, argv);

    std::vector<int> v;

    setUp([&]() {
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
    });

    tearDown([&]() {
        v.clear();
    });

    test("After pushing back 3, 4 and 5, v has size 3", [&]() {
        expect(v.size() == 3);
    });

    test("After pushing back 3, 4 and 5, v is equal to {3, 4, 5}", [&]() {
        expect(v == std::vector<int>{3, 4, 5});
    });

    test("After pushing back 3, 4 and 5, v does not contain '6'", [&]() {
        expect(std::find(v.begin(), v.end(), 6) == v.end());
    });

    return kktest::finalizeTesting();
}
