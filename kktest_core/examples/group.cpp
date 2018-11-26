#include <vector>

#include <kktest>

void kkTestCase() {
    std::vector<int> v;

    kkSetUp([&]() {
        v = std::vector<int>{}; // always start with a clean vector
    });

    kkTest("Vector is initially empty", [&] {
        kkExpect(v.empty());
        kkExpect(v.size() == 0);
    });

    kkTest("After one push_back, vector is not empty anymore", [&]() {
        v.push_back(3);
        kkExpect(!v.empty());
        kkExpect(v.size() != 0);
    });

    kkGroup("After inserting 5 elements", [&]() {
        kkSetUp([&]() {
            v.insert(v.end(), {1, 2, 3, 4, 5});
        });

        kkTest("Size of the vector is 5", [&]() {
            kkExpect(v.size() == 5);
        });

        kkTest("v.at(4) does not throw", [&]() {
            try {
                v.at(4);
            } catch(...) {
                kkFail("It did throw.");
            }
        });
    });

    kkTest("v.at(4) throws on empty vector", [&]() {
        try {
            v.at(4);
            kkFail("Did not throw.");
        } catch(...) { /* All ok; */ }
    });
}
