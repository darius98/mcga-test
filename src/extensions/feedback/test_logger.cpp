#include "test_logger.hpp"

#include <iomanip>
#include <numeric>

#include <termcolor/termcolor.hpp>

#include "core/time_tick.hpp"

namespace mcga::test::feedback {

TestLogger::TestLogger(std::ostream& stream, bool liveLogging)
        : stream(stream), liveLogging(liveLogging) {
}

void TestLogger::onTestExecutionStart(const Test& test) {
    runningTests.insert(test.getId());
    updateVolatileLine(test);
}

void TestLogger::onTestExecutionFinish(const Test& test) {
    runningTests.erase(test.getId());
    updateVolatileLine(test);

    if (!test.isExecuted()) {
        return;
    }

    if (test.getTotalTimeTicks() != -1.0) {
        totalTimeTicks += test.getTotalTimeTicks();
    }
    if (test.isPassed()) {
        ++passedTests;
    } else {
        ++failedTests;
        if (test.isOptional()) {
            ++failedOptionalTests;
        }
    }
    ++loggedTests;

    printTestMessage(test);
}

void TestLogger::printFinalInformation() {
    if (isLastLineVolatile) {
        stream << "\r";
        stream.flush();
    }
    stream << "\n";
    stream << "Tests passed: " << termcolor::green << passedTests
           << termcolor::reset << "\n";
    stream << "Tests failed: ";
    if (failedTests == 0) {
        stream << termcolor::green << failedTests << termcolor::reset;
    } else if (failedTests == failedOptionalTests) {
        stream << termcolor::yellow << failedTests << termcolor::reset;
    } else {
        stream << termcolor::red << failedTests << termcolor::reset;
    }
    if (failedOptionalTests != 0) {
        stream << " (" << termcolor::yellow << failedOptionalTests
               << termcolor::reset << " "
               << (failedOptionalTests == 1 ? "was" : "were") << " optional)";
    }
    stream << "\n";
    stream << "Total recorded testing time: " << std::fixed
           << std::setprecision(3) << totalTimeTicks << " ticks ("
           << TimeTicksToNanoseconds(totalTimeTicks).count() * 1.0
        * std::milli::den / std::nano::den
           << " ms)\n";
    isLastLineVolatile = false;
}

void TestLogger::printWarning(const Warning& warning) {
    if (isLastLineVolatile) {
        stream << "\r";
        stream.flush();
    }
    stream << termcolor::yellow << "Warning: " << warning.message << "\n";
    if (warning.context.has_value()) {
        stream << "\tat " << warning.context->fileName << ":"
               << warning.context->line << ":" << warning.context->column
               << "\n";
    }
    for (const auto& note: warning.notes) {
        stream << "\tNote: " << note.message;
        if (note.context.has_value()) {
            stream << " at " << note.context->fileName << ":"
                   << note.context->line << ":" << note.context->column;
        }
        stream << "\n";
    }
    stream << termcolor::reset;
}

void TestLogger::printTestPassedOrFailedToken(const Test& test) {
    stream << "[";
    if (test.isPassed()) {
        stream << termcolor::green << "P" << termcolor::reset;
    } else if (test.isOptional()) {
        stream << termcolor::yellow << "F" << termcolor::reset;
    } else {
        stream << termcolor::red << "F" << termcolor::reset;
    }
    stream << "]";
}

void TestLogger::printTestAndGroupsDescription(const Test& test) {
    std::vector<std::string> groupDescriptions;
    GroupPtr group = test.getGroup();
    while (group != nullptr) {
        std::string groupDescription = group->getDescription();
        if (!groupDescription.empty()) {
            groupDescriptions.push_back(groupDescription);
        }
        group = group->getParentGroup();
    }
    std::string groupDescription
      = std::accumulate(groupDescriptions.rbegin(),
                        groupDescriptions.rend(),
                        std::string(""),
                        [](const std::string& a, const std::string& b) {
                            return a.empty() ? b : (a + "::" + b);
                        });
    if (!groupDescription.empty()) {
        groupDescription += "::";
    }
    stream << groupDescription << test.getDescription();
}

void TestLogger::printTestExecutionTime(const Test& test) {
    if (test.getAvgTimeTicksForExecution() != -1.0) {
        stream
          << std::fixed << std::setprecision(3)
          << (test.getNumAttempts() > 1 ? "~ " : "")
          << test.getAvgTimeTicksForExecution() << " ticks ("
          << (test.getNumAttempts() > 1 ? "~" : "")
          << TimeTicksToNanoseconds(test.getAvgTimeTicksForExecution()).count()
            * 1.0 * std::milli::den / std::nano::den
          << " ms)";
    } else {
        stream << "(unknown time)";
    }
}

void TestLogger::printTestAttemptsInfo(const Test& test) {
    stream << "(passed: " << test.getNumPassedAttempts() << "/"
           << test.getNumAttempts();
    if (!test.isPassed()) {
        stream << ", required: " << test.getNumRequiredPassedAttempts();
    }
    stream << ")";
}

void TestLogger::printTestFailure(const Test::ExecutionInfo& info) {
    stream << "\n";
    auto failure = info.failure;
    // TODO: This should be somewhere else (in utils maybe?)
    size_t pos = 0;
    while ((pos = failure.find('\n', pos)) != std::string::npos) {
        failure.replace(pos, 1, "\n\t");
        pos += 2;
    }
    stream << termcolor::red;
    if (info.failureContext.has_value()) {
        stream << info.failureContext->verb << " at "
               << info.failureContext->fileName << ":"
               << info.failureContext->line << ":"
               << info.failureContext->column << "\n";
    }
    stream << "\t" << failure << termcolor::reset;
}

void TestLogger::printTestMessage(const Test& test) {
    if (isLastLineVolatile) {
        stream << "\r";
        stream.flush();
    }

    printTestPassedOrFailedToken(test);
    stream << " ";
    printTestAndGroupsDescription(test);
    stream << " - ";
    printTestExecutionTime(test);
    if (test.getNumAttempts() > 1) {
        stream << ' ';
        printTestAttemptsInfo(test);
    }
    const auto lastFailure = test.getLastFailure();
    if (!test.isPassed() && lastFailure.has_value()) {
        printTestFailure(lastFailure.value());
    }
    stream << "\n";
    isLastLineVolatile = false;
}

void TestLogger::updateVolatileLine(const Test& test) {
    if (!liveLogging) {
        return;
    }

    if (isLastLineVolatile) {
        stream << "\r";
        stream.flush();
    }

    if (runningTests.size() == 1 && *runningTests.begin() == test.getId()) {
        stream << "[" << termcolor::yellow << "." << termcolor::reset << "] ";
        printTestAndGroupsDescription(test);
        if (test.getNumAttempts() > 1) {
            stream << " - running attempt " << test.getExecutions().size() + 1
                   << " of " << test.getNumAttempts() << ", passed "
                   << test.getNumPassedAttempts() << " so far...";
        } else {
            stream << " - running...";
        }
    } else if (runningTests.size() > 1) {
        stream << runningTests.size() << " tests running...";
    } else if (runningTests.size() == 1) {
        stream << "One test running...";
    } else {
        stream << "No tests running...";
    }
    stream.flush();

    isLastLineVolatile = true;
}

}  // namespace mcga::test::feedback
