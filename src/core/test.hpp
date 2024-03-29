#pragma once

#include <optional>

#include "group.hpp"
#include "mcga/test.hpp"

namespace mcga::test {

class ExtensionApi;

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
        enum Status : char { PASSED = 0, FAILED = 1, SKIPPED = 2 } status = PASSED;

        String verb;

        /** Human-readable message for the status of the test (explanation for
         * failure or skip etc.). */
        String message = "OK";

        /** Context where the status changed, in case it's not PASSED. Only
         * when available (e.g. it's not available for tests failed with
         * timeout). */
        std::optional<Context> context = std::nullopt;

        /** Number of time ticks the execution took.
         *
         * Is `-1` by default, signaling that the number of time ticks was not
         * computable for this execution. */
        double timeTicks = -1.0;

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

        [[nodiscard]] bool isPassed() const;
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
    [[nodiscard]] const String& getDescription() const;

    /** See TestConfig#sourceLocation. */
    [[nodiscard]] const Context& getContext() const;

    /** See TestConfig#optional. */
    [[nodiscard]] bool isOptional() const;

    /** See TestConfig#timeTicksLimit. */
    [[nodiscard]] double getTimeTicksLimit() const;

    /** See TestConfig#attempts. */
    [[nodiscard]] int getNumAttempts() const;

    /** See TestConfig#numRequiredPassedAttempts. */
    [[nodiscard]] int getNumRequiredPassedAttempts() const;

    /** The group that contains this test. */
    [[nodiscard]] GroupPtr getGroup() const;

    /** Globally unique, incremental identifier of the test. */
    [[nodiscard]] int getId() const;

    /** Execute the test body.
     *
     * Should only be called by Executor and its sub-/helper-classes, after
     * running all required set-ups and before all required tear-downs.
     */
    [[nodiscard]] const Executable& getBody() const;

    /** Check whether the number of executions registered is the same as
     * getNumAttempts(). */
    [[nodiscard]] bool isFinished() const;

    /** Check whether the execution info marks the test as passed. */
    [[nodiscard]] bool isPassed() const;

    /** Check whether the execution info marks the test as skipped. For a test
     * to be considered skipped, all executions must be marked as passed or
     * skipped. */
    [[nodiscard]] bool isSkipped() const;

    /** Check whether the execution info marks the test as passed or failed. A
     * test is failed if it's neither passed nor skipped (at least one execution
     * is failed). */
    [[nodiscard]] bool isFailed() const;

    /** The number of attempts executed (passed, failed or skipped). */
    [[nodiscard]] int getNumExecutedAttempts() const;

    /** The number of attempts passed. */
    [[nodiscard]] int getNumPassedAttempts() const;

    /** The number of attempts skipped. */
    [[nodiscard]] int getNumSkippedAttempts() const;

    /** The average number of time ticks per execution (only counting executions
     * where the number of time ticks is computable).
     *
     * Note: If the time is not computable for any execution, returns -1. */
    [[nodiscard]] double getAvgTimeTicksForExecution() const;

    /** The total number of time ticks used (only counting executions where the
     * number of time ticks is computable).
     *
     * Note: If the time is not computable for any execution, returns -1. */
    [[nodiscard]] double getTotalTimeTicks() const;

    [[nodiscard]] const ExecutionInfo& getLastExecution() const;

    [[nodiscard]] const std::optional<Test::ExecutionInfo>&
      getLastFailedExecution() const;

    [[nodiscard]] const std::optional<Test::ExecutionInfo>&
      getLastSkippedExecution() const;

    /** Add an execution to the test. */
    void addExecution(ExecutionInfo info, ExtensionApi* api);

  private:
    Executable body;
    GroupPtr group;
    int id;

    int numExecutedAttempts = 0;
    int numPassedAttempts = 0;
    int numSkippedAttempts = 0;
    int numTimedAttempts = 0;
    double trackedExecutionTimeTicks = 0;
    ExecutionInfo lastExecution;
    std::optional<Test::ExecutionInfo> lastFailedExecution = std::nullopt;
    std::optional<Test::ExecutionInfo> lastSkippedExecution = std::nullopt;
};

}  // namespace mcga::test
