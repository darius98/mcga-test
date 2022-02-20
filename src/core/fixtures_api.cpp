#include "mcga/test.hpp"

#include "aa_list.hpp"
#include "export.hpp"

namespace mcga::test::internal {

MCGA_TEST_EXPORT void register_test_in_fixture(FixtureData* fixture,
                                               const char* fixture_name,
                                               FixtureTest* fixture_test) {
    if (fixture->name == nullptr) {
        register_test_case(fixture_name, fixture->body);
        fixture->name = fixture_name;
    }
    list_push_back(fixture->head, fixture->tail, fixture_test);
}

}  // namespace mcga::test::internal
