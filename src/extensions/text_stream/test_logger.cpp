#include "test_logger.hpp"

#include <iomanip>
#include <numeric>

#include "color.hpp"

#include "core/time_tick.hpp"
#include "extensions/filter/test_description.hpp"

namespace mcga::test {

TestLogger::TestLogger(std::ostream& stream,
                       bool liveLogging,
                       bool printSkipped)
        : stream(stream), liveLogging(liveLogging), printSkipped(printSkipped) {
}

void TestLogger::onTestExecutionStart(const Test& test) {
    runningTests.insert(test.getId());
    updateVolatileLine(test);

    // Flush before a test. If not, buffered output might get printed by both
    // the orchestrator and the worker process in non-smooth execution.
    std::cout.flush();
}

void TestLogger::onTestExecutionFinish(const Test& test) {
    runningTests.erase(test.getId());
    updateVolatileLine(test);

    if (!test.isFinished()) {
        return;
    }

    if (test.getTotalTimeTicks() != -1.0) {
        totalTimeTicks += test.getTotalTimeTicks();
    }
    if (test.isPassed()) {
        ++passedTests;
    } else if (test.isSkipped()) {
        ++skippedTests;
    } else {
        ++failedTests;
        if (test.isOptional()) {
            ++failedOptionalTests;
        }
    }
    ++loggedTests;

    if (!test.isSkipped() || printSkipped) {
        printTestMessage(test);
    }
}

void TestLogger::printFinalInformation() {
    clearVolatileLine();
    if (loggedTests > 0) {
        stream << "\n";
    }
    stream << "Tests passed: " << green << passedTests << reset << "\n";
    stream << "Tests failed: ";
    if (failedTests == 0) {
        stream << green << failedTests << reset;
    } else if (failedTests == failedOptionalTests) {
        stream << yellow << failedTests << reset;
    } else {
        stream << red << failedTests << reset;
    }
    if (failedOptionalTests != 0) {
        stream << " (" << yellow << failedOptionalTests << reset << " "
               << (failedOptionalTests == 1 ? "was" : "were") << " optional)";
    }
    stream << "\n";
    if (skippedTests != 0) {
        stream << "Tests skipped: " << yellow << skippedTests << reset << "\n";
    }
    stream << "Total recorded testing time: " << std::fixed
           << std::setprecision(3) << totalTimeTicks << " ticks ("
           << TimeTicksToNanoseconds(totalTimeTicks).count() * 1.0
        * std::milli::den / std::nano::den
           << " ms)\n";
}

void TestLogger::printWarning(const Warning& warning) {
    clearVolatileLine();
    stream << yellow << "Warning: " << warning.message.c_str() << "\n";
    if (warning.context.has_value()) {
        stream << "\tat " << warning.context->fileName.c_str() << ":"
               << warning.context->line << ":" << warning.context->column
               << "\n";
    }
    for (const auto& note: warning.notes) {
        stream << "\tNote: ";
        switch (note.type) {
            case WarningNoteType::TEST: {
                stream << "While running test ";
                break;
            }
            case WarningNoteType::GROUP: {
                stream << "In group ";
                break;
            }
            case WarningNoteType::SET_UP: {
                stream << "While running setUp";
                break;
            }
            case WarningNoteType::TEAR_DOWN: {
                stream << "While running tearDown";
                break;
            }
            case WarningNoteType::CLEANUP: {
                stream << "While running cleanup";
                break;
            }
            default: break;
        }
        stream << note.message.c_str();
        if (note.context.has_value()) {
            stream << " at " << note.context->fileName.c_str() << ":"
                   << note.context->line << ":" << note.context->column;
        }
        stream << "\n";
    }
    stream << reset;
}

void TestLogger::clearVolatileLine() {
    if (isLastLineVolatile) {
        stream << "\r\033[K" << std::flush;
    }
    isLastLineVolatile = false;
}

void TestLogger::printTestStatus(const Test& test) {
    stream << "[";
    if (test.isPassed()) {
        stream << green << "P" << reset;
    } else if (test.isSkipped() || test.isOptional()) {
        stream << yellow << (test.isSkipped() ? "S" : "F") << reset;
    } else {
        stream << red << "F" << reset;
    }
    stream << "]";
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
    std::string message = info.message.c_str();
    // TODO: This should be somewhere else (in utils maybe?)
    size_t pos = 0;
    while ((pos = message.find('\n', pos)) != std::string::npos) {
        message.replace(pos, 1, "\n\t");
        pos += 2;
    }
    stream << (info.status == Test::ExecutionInfo::SKIPPED ? yellow : red);
    if (info.context.has_value()) {
        stream << info.verb.c_str() << " at " << info.context->fileName.c_str()
               << ":" << info.context->line << ":" << info.context->column;
    }
    if (!message.empty()) {
        if (info.context.has_value()) {
            stream << "\n";
        }
        stream << "\t" << message;
    }
    stream << reset;
}

void TestLogger::printTestMessage(const Test& test) {
    clearVolatileLine();

    printTestStatus(test);
    stream << " " << getTestFullDescription(test) << " - ";
    printTestExecutionTime(test);
    if (test.getNumAttempts() > 1) {
        stream << ' ';
        printTestAttemptsInfo(test);
    }
    const auto& execution = test.isFailed() ? test.getLastFailedExecution()
                                            : test.getLastSkippedExecution();
    if (execution.has_value()) {
        printTestFailure(execution.value());
    }
    stream << "\n";
}

void TestLogger::updateVolatileLine(const Test& test) {
    if (!liveLogging || !is_terminal(stream)) {
        return;
    }
    clearVolatileLine();
    if (runningTests.size() == 1 && *runningTests.begin() == test.getId()) {
        stream << "[" << yellow << "." << reset << "] "
               << getTestFullDescription(test);
        if (test.getNumAttempts() > 1) {
            stream << " - running attempt " << test.getNumExecutedAttempts() + 1
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

}  // namespace mcga::test
