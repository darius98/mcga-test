#include <kktest>

int main() {
    std::vector<int> v;

    setUp([&]() {
        v = std::vector<int>{}; // always start with a clean vector
    });

    test("Vector is initially empty", [&] {
        expect(v.empty());
        expect(v.size() == 0);
    });

    test("After one push_back, vector is not empty anymore", [&]() {
        v.push_back(3);
        expect(!v.empty());
        expect(v.size() != 0);
    });

    group("After inserting 5 elements", [&]() {
        setUp([&]() {
            v.insert(v.end(), {1, 2, 3, 4, 5});
        });

        test("Size of the vector is 5", [&]() {
            expect(v.size() == 5);
        });

        test("v.at(4) does not throw", [&]() {
            try {
                v.at(4);
            } catch(...) {
                fail("It did throw.");
            }
        });
    });

    test("v.at(4) throws on empty vector", [&]() {
        try {
            v.at(4);
            fail("Did not throw.");
        } catch(...) { /* All ok; */ }
    });

    return kktest::finalizeTesting();
}
