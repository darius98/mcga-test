#pragma once

#include <optional>
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
        enum Status : char { PASSED = 0, FAILED = 1 } status = PASSED;

        /** Number of time ticks the execution took.
         *
         * Is `-1` by default, signaling that the number of time ticks was not
         * computable for this execution. */
        double timeTicks = -1.0;

        /** Human-readable message for the status of the test (explanation for
         * failure or skip etc.). */
        String message = "OK";

        /** Context where the status changed, in case it's not PASSED. Only
         * when available (e.g. it's not available for tests failed with
         * timeout). */
        std::optional<Context> context = std::nullopt;

        /** Shorthand method for marking the test execution as failed.
         *
         * Note: if this method is called multiple times, subsequent calls do
         * NOT overwrite the first failure message. */
        void fail(const String& failureMessage,
                  std::optional<Context> failureContext = std::nullopt);

        /** Shorthand for merging one execution info into another.
         *
         * Note: if the execution is already failed, does nothing. */
        void merge(ExecutionInfo&& other);
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
    const String& getDescription() const;

    /** See TestConfig#sourceLocation. */
    const Context& getContext() const;

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
    const Executable& getBody() const;

    /** Check whether the number of executions registered is the same as
     * getNumAttempts(). */
    bool isFinished() const;

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
    std::optional<Test::ExecutionInfo> getLastFailure() const;

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
