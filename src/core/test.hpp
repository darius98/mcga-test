#pragma once

#include <vector>

#include "group.hpp"
#include "mcga/test.hpp"

namespace mcga::test {

/** Class holding the internal representation of a test.
 *
 * This class should only be instantiated once for every call to test() from
 * the API of the library, as an upgrade to the TestConfig instance passed to
 * the test() function. This class' copy constructor and assignment operator are
 * explicitly deleted, as it should never need to be copied throughout the
 * lifecycle of a test.
 *
 * This class privately inherits TestConfig because TestConfig exposes public
 * configuration mutability (TestConfig#setDescription(),
 * TestConfig#setOptional() etc.). */
class Test : private TestConfig {
  public:
    /** Simple helper structure that holds the execution information for a
     * test. */
    struct ExecutionInfo {
        /** Number of time ticks the execution took.
         *
         * Is `-1` by default, signaling that the number of time ticks was not
         * computable for this execution. */
        double timeTicks = -1.0;

        /** Whether the test passed or not. */
        bool passed = true;

        /** Human-readable failure message in case the test failed, or empty
         * string if the test passed. */
        std::string failure = "";

        /** Default constructor. */
        ExecutionInfo() = default;

        /** Shorthand method for marking the test execution as failed and saving
         * a failure message.
         *
         * Note: if this method is called multiple times, subsequent calls do
         * NOT overwrite the first failure message. */
        void fail(const std::string& _failure, double timeTicks = -1.0);
    };

    /** Default constructor from a TestConfig and the metadata received from the
     * testing Driver. */
    Test(TestConfig config, Executable body, GroupPtr group, int id);

    Test(Test&&) = default;
    Test& operator=(Test&&) = default;

    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;

    ~Test() = default;

    /** See TestConfig#description. */
    const std::string& getDescription() const;

    /** See TestConfig#optional. */
    bool isOptional() const;

    /** See TestConfig#timeTicksLimit. */
    double getTimeTicksLimit() const;

    /** See TestConfig#attempts. */
    std::size_t getNumAttempts() const;

    /** See TestConfig#numRequiredPassedAttempts. */
    std::size_t getNumRequiredPassedAttempts() const;

    /** The group that contains this test. */
    GroupPtr getGroup() const;

    /** The groups, in order from the global scope to the nearest, that
     * contain this test. */
    std::vector<GroupPtr> getGroupStack() const;

    /** Globally unique, incremental identifier of the test. */
    int getId() const;

    /** Execute the test body.
     *
     * Should only be called by Executor and its sub-/helper-classes, after
     * running all required set-ups and before all required tear-downs.
     */
    void run() const;

    /** Check whether the number of executions registered is the same as
     * getNumAttempts(). */
    bool isExecuted() const;

    /** Check whether the execution Info marks the test as passed or failed. */
    bool isPassed() const;

    /** The number of attempts passed. */
    std::size_t getNumPassedAttempts() const;

    /** The average number of time ticks per execution (only counting executions
     * where the number of time ticks is computable).
     *
     * Note: If the time is not computable for any execution, returns -1. */
    double getAvgTimeTicksForExecution() const;

    /** The total number of time ticks used (only counting executions where the
     * number of time ticks is computable).
     *
     * Note: If the time is not computable for any execution, returns -1. */
    double getTotalTimeTicks() const;

    /** Get the error message for the last failed execution, or empty string if
     * all executions passed. */
    std::string getLastFailure() const;

    /** Get the array of executions. */
    const std::vector<ExecutionInfo>& getExecutions() const;

    /** Add an execution to the test. */
    void addExecution(const ExecutionInfo& info);

  private:
    Executable body;
    GroupPtr group;
    int id;

    std::vector<ExecutionInfo> executions;
};

}  // namespace mcga::test
