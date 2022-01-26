#pragma once

#include "aa_list.hpp"
#include "memory.hpp"

namespace mcga::test {

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
    class Ptr {
        Group* raw = nullptr;

        explicit Ptr(Group* group);

        void del();

      public:
        explicit Ptr();
        Ptr(std::nullptr_t) noexcept {
        }
        Ptr(const Ptr& other) noexcept;
        Ptr(Ptr&& other) noexcept;
        Ptr& operator=(const Ptr& other) noexcept;
        Ptr& operator=(Ptr&& other) noexcept;
        ~Ptr();

        Group* operator->() noexcept;
        const Group* operator->() const noexcept;
        Group& operator*() noexcept;
        const Group& operator*() const noexcept;

        bool operator==(std::nullptr_t) const noexcept;
        bool operator!=(std::nullptr_t) const noexcept;

        friend class Group;
    };

    static Ptr
      make(GroupConfig config, Context context, Ptr parentGroup, int id);

    /** See GroupConfig#description. */
    [[nodiscard]] const String& getDescription() const;

    /** See GroupConfig#sourceLocation. */
    [[nodiscard]] const Context& getContext() const;

    /** See GroupConfig#optional. */
    [[nodiscard]] bool isOptional() const;

    /** Globally unique, incremental identifier of the group. */
    [[nodiscard]] int getId() const;

    /** Pointer to the Group that contains this, or `nullptr` if the current
     * group represents a TestCase. */
    [[nodiscard]] Ptr getParentGroup() const;

    [[nodiscard]] bool hasParentGroup() const;

    /** Add a set-up function to this group.
     *
     * This method is called once for every setUp() call from the
     * global API, on the instance that is currently on top of the group
     * stack. */
    void addSetUp(Executable func);

    template<class Callable>
    void forEachSetUp(Callable callable) const {
        for (const auto& setUp: setUps) {
            if (!callable(setUp)) {
                break;
            }
        }
    }

    /** Add a tear-down function to this group.
     *
     * This method is called once for every tearDown() call from the
     * global API, on the instance that is currently on top of the group
     * stack. */
    void addTearDown(Executable func);

    /** Call the tear-down functions of this group, in reverse order. */
    template<class Callable>
    void forEachTearDown(Callable callable) const {
        for (const auto& tearDown: tearDowns) {
            if (!callable(tearDown)) {
                break;
            }
        }
    }

  private:
    int refCount = 0;
    int id;
    Ptr parentGroup;

    Context context;
    ExecutableList setUps;
    ExecutableList tearDowns;

    /** Default constructor from the upgraded GroupConfig and extra metadata
     * received from the testing Driver. Very similar to Test#Test().*/
    Group(GroupConfig config, Context context, Ptr parentGroup, int id);

    friend class Group::Ptr;
};

using GroupPtr = Group::Ptr;

}  // namespace mcga::test
