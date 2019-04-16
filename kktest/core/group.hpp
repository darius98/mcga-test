#pragma once

#include <memory>

#include "kktest/core/include/kktest.hpp"

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
    typedef std::shared_ptr<Group> Ptr;

    /** Default constructor from the upgraded GroupConfig and extra metadata
     * received from the testing Driver. Very similar to Test#Test().*/
    Group(GroupConfig config, Ptr parentGroup, int id);

    /** See GroupConfig#description. */
    std::string getDescription() const;

    /** See GroupConfig#optional. */
    bool isOptional() const;

    /** Globally unique, incremental identifier of the group. */
    int getId() const;

    /** Pointer to the Group that contains this, or `nullptr` if the current
     * group represents a TestCase. */
    Ptr getParentGroup() const;

    /** Add a set-up function to this group.
     *
     * This method is called once for every setUp(Executable) call from the
     * global API, on the instance that is currently on top of the group
     * stack.
     *
     * Note: This throws on being called the second time for the same group. */
    void addSetUp(Executable func);

    /** Returns whether this group has a setUp.
     *
     * Is true only if addSetUp() was called on this instance. */
    bool hasSetUp() const;

    /** Call the set-up function of this group. */
    void setUp() const;

    /** Add a tear-down function to this group.
     *
     * This method is called once for every tearDown(Executable) call from the
     * global API, on the instance that is currently on top of the group
     * stack.
     *
     * Note: This throws on being called the second time for the same group. */
    void addTearDown(Executable func);

    /** Returns whether this group has a tearDown.
     *
     * Is true only if addTearDown() was called on this instance. */
    bool hasTearDown() const;

    /** Call the tear-down function of this group. */
    void tearDown() const;

    /** Increase the number of tests that have started within this group (and
     * all sub-groups of it.
     *
     * Should only be called within the implementation of the test() function,
     * only on the instance maintaining the direct Group container of the added
     * test (the top of the group stack), since it recursively updates all
     * parents of the instance where it's called.
     *
     * Together with setStartedAllTests(), addFinishedTest() and
     * finishedAllTests() it forms the Group's API for defining a moment when
     * this Group can be cleaned up (it is no longer needed by the Driver). */
    void addStartedTest();

    /** Mark a flag that all tests within this group have started. */
    void setStartedAllTests();

    /** Increase the number of tests that have finished execution within this
     * group. */
    void addFinishedTest();

    /** Check if all tests within this group and all subgroups are finished.
     *
     * This is `true` iff the flag from setStartedAllTests() was set and the
     * counters for started & finished tests are equal. */
    bool finishedAllTests() const;

  private:
    Ptr parentGroup;
    int id;

    int testsStarted = 0;
    bool allTestsStarted = false;
    int testsFinished = 0;

    Executable setUpFunc;
    Executable tearDownFunc;
};

typedef Group::Ptr GroupPtr;

}  // namespace mcga::test
