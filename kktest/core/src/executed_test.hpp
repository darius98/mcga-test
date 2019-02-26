#ifndef KKTEST_CORE_SRC_EXECUTED_TEST_HPP_
#define KKTEST_CORE_SRC_EXECUTED_TEST_HPP_

#include <vector>

#include "kktest/core/src/test.hpp"

namespace kktest {

/** Immutable class representing a Test that has finished its execution, and the
 * Executor has provided it with the execution information.
 *
 * This class should always be instantiated from a rvalue Test instance, since
 * a Test is basically _upgraded_ to an ExecutedTest, and should not be used
 * after this. */
class ExecutedTest : public Test {
 public:
    /** Simple helper structure that holds the execution information for a
     * test. */
    struct Info {
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
        Info() = default;

        /** Shorthand constructor for a failed execution where no other
         * information except a failure message could be computed (e.g. KBS). */
        explicit Info(std::string&& failure);

        /** Shorthand method for marking the test execution as failed and saving
         * a failure message.
         *
         * Note: if this method is called multiple times, subsequent calls do
         * NOT overwrite the first failure message. */
        void fail(const std::string& failure);
    };

    /** Upgrade a Test to an ExecutedTest with the use of the execution Info
     * result for all executions.
     *
     * Since this process only happens once for a test, and since the Test
     * instance should no longer be needed after obtaining an ExecutedTest
     * instance, this constructor moves the Test. */
    ExecutedTest(Test&& test, std::vector<Info>&& executions);

    /** Check whether the execution Info marks the test as passed or failed. */
    bool isPassed() const;

    /** The number of attempts passed. */
    int getNumPassedAttempts() const;

    /** The average number of time ticks per execution (only counting executions
     * where the number of time ticks is computable). */
    double getAvgTimeTicksForExecution() const;

    /** The total number of time ticks used (only counting executions where the
     * number of time ticks is computable). */
    double getTotalTimeTicks() const;

    /** Get the error message for the last failed execution, or empty string if
     * all executions passed. */
    std::string getLastFailure() const;

    /** Get the array of executions. */
    const std::vector<Info>& getExecutions() const;

 private:
    std::vector<Info> executions;

    // Cached values.
    mutable int numPassedExecutions = -1;
    mutable double totalTimeTicks = -2; // -1=un-computable. -2=not calculated
    mutable double avgTimeTicks = -2; // -1=un-computable. -2=not calculated
};

}

#endif
