#include "mcga/test.hpp"

#include "aa_list.hpp"
#include "config.hpp"
#include "export.hpp"
#include "memory.hpp"

// TODO: Remove dynamic allocations.

namespace mcga::test::internal {

struct FixtureTest {
    const char* description;
    void (*body)(void*);
    Context context;
};

// TODO: Remove circular dependencies here
struct FixtureTestAllocator;

using FixtureTestList = List<FixtureTest, FixtureTestAllocator>;

static BufferFor<FixtureTestList::node, 32> staticFixtureTests;

struct FixtureTestAllocator {
    [[nodiscard]] static void* allocate() {
        return staticFixtureTests.allocate();
    }
    static void deallocate(void* ptr) {
        staticFixtureTests.deallocate(ptr);
    }
};

struct FixtureRegisterer {
    void (*body)();
    const char* name = nullptr;
    List<FixtureTest, FixtureTestAllocator> tests;
};

static BufferFor<FixtureRegisterer, 32> staticFixtures;

MCGA_TEST_EXPORT void* register_fixture(void (*fixture_body)()) {
    void* ptr = staticFixtures.allocate();
    new (ptr) FixtureRegisterer{.body = fixture_body};
    return ptr;
}

MCGA_TEST_EXPORT int register_test_in_fixture(void* raw_registerer,
                                              const char* fixture_name,
                                              void (*test_body)(void*),
                                              const char* test_description,
                                              Context context) {
    const auto registerer = static_cast<FixtureRegisterer*>(raw_registerer);
    if (registerer->name == nullptr) {
        register_test_case(fixture_name, registerer->body);
        registerer->name = fixture_name;
    }
    registerer->tests.push_back(FixtureTest{
      .description = test_description,
      .body = test_body,
      .context = std::move(context),
    });
    return 0;
}

MCGA_TEST_EXPORT void run_fixture_tests(void* raw_registerer, void* fixture) {
    const auto registerer = static_cast<FixtureRegisterer*>(raw_registerer);
    for (const auto& registerer_test: registerer->tests) {
        test(registerer_test.description,
             Executable{
               [fixture, &registerer_test]() {
                   registerer_test.body(fixture);
               },
               std::move(registerer_test.context),
             });
    }
}

}  // namespace mcga::test::internal
