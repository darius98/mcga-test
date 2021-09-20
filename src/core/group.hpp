#pragma once

#include <memory>
#include <vector>

#include "mcga/test.hpp"

namespace mcga::test {

struct UserTestExecutable {
    Executable func;
    Context context;
};

/** Internal representation of a group.
 *
 * This class should only be instantiated once per call to the group() function
 * from the public API. The current convention is that every instance is created
 * by the testing Driver, but is created as a std::shared_ptr, to avoid
 * premature deletion (extensions currently have direct access to the pointer,
 * and could have invalid access if the Driver decides to delete the instance).
 *
 * Same as Test, this is an _upgrade_ to the GroupConfig received by the group()
 * function, and therefore inherits privately from GroupConfig (same as Test,
 * config items are immutable once received into the system). Unlike Test,
 * however, a Group is not _immutable_ (e.g. addSetUp(), addTearDown() and
 * others). */
class Group : private GroupConfig {
  public:
    /** The pointer type that should be passed around when working with a
     * Group. */
    using Ptr = std::shared_ptr<Group>;

    /** Default constructor from the upgraded GroupConfig and extra metadata
     * received from the testing Driver. Very similar to Test#Test().*/
    Group(GroupConfig config, Ptr parentGroup, int id);

    /** See GroupConfig#description. */
    [[nodiscard]] const std::string& getDescription() const;

    /** See GroupConfig#sourceLocation. */
    [[nodiscard]] const Context& getContext() const;

    /** See GroupConfig#optional. */
    [[nodiscard]] bool isOptional() const;

    /** Globally unique, incremental identifier of the group. */
    [[nodiscard]] int getId() const;

    /** Pointer to the Group that contains this, or `nullptr` if the current
     * group represents a TestCase. */
    [[nodiscard]] Ptr getParentGroup() const;

    /** Add a set-up function to this group.
     *
     * This method is called once for every setUp(Executable) call from the
     * global API, on the instance that is currently on top of the group
     * stack. */
    void addSetUp(UserTestExecutable func);

    template<class Callable>
    void forEachSetUp(Callable callable) const {
        for (const auto& setUp: setUpFuncs) {
            if (!callable(setUp)) {
                break;
            }
        }
    }

    /** Add a tear-down function to this group.
     *
     * This method is called once for every tearDown(Executable) call from the
     * global API, on the instance that is currently on top of the group
     * stack. */
    void addTearDown(UserTestExecutable func);

    /** Call the tear-down functions of this group, in reverse order. */
    template<class Callable>
    void forEachTearDown(Callable callable) const {
        for (auto it = tearDownFuncs.rbegin(); it != tearDownFuncs.rend();
             it++) {
            if (!callable(*it)) {
                break;
            }
        }
    }

  private:
    Ptr parentGroup;
    int id;

    std::vector<UserTestExecutable> setUpFuncs;
    std::vector<UserTestExecutable> tearDownFuncs;
};

using GroupPtr = Group::Ptr;

}  // namespace mcga::test
