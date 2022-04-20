#include "extensions/text_stream.hpp"

#include <iomanip>
#include <iostream>
#include <memory>

#include <unistd.h>

#include "core/time_tick.hpp"
#include "test_description.hpp"

namespace {

bool is_terminal(const std::ostream& stream) {
    if (&stream == &std::cout) {
        return isatty(fileno(stdout));
    }
    if (&stream == &std::cerr) {
        return isatty(fileno(stderr));
    }
    return false;
}

void write_if_colorized(std::ostream& stream, const char* ansi_seq) {
    if (is_terminal(stream)) {
        stream << ansi_seq;
    }
}

std::ostream& reset(std::ostream& stream) {
    write_if_colorized(stream, "\033[00m");
    return stream;
}

std::ostream& red(std::ostream& stream) {
    write_if_colorized(stream, "\033[31m");
    return stream;
}

std::ostream& green(std::ostream& stream) {
    write_if_colorized(stream, "\033[32m");
    return stream;
}

std::ostream& yellow(std::ostream& stream) {
    write_if_colorized(stream, "\033[33m");
    return stream;
}

}  // namespace

namespace mcga::test {

StdoutLoggingExtension::StdoutLoggingExtension(bool printSkipped,
                                               bool liveLogging)
        : stream(std::cout), liveLogging(liveLogging),
          printSkipped(printSkipped) {
}

void StdoutLoggingExtension::beforeTestExecution(const Test& test) {
    runningTests.insert(test.getId());
    updateVolatileLine(test);

    // Flush before a test. If not, buffered output might get printed by both
    // the orchestrator and the worker process in non-smooth execution.
    std::cout.flush();
}

void StdoutLoggingExtension::afterTestExecution(const Test& test) {
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

void StdoutLoggingExtension::destroy() {
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
           << static_cast<double>(
                TimeTicksToNanoseconds(totalTimeTicks).count())
        * std::milli::den / std::nano::den
           << " ms)\n";
}

void StdoutLoggingExtension::onWarning(const Warning& warning) {
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

void StdoutLoggingExtension::clearVolatileLine() {
    if (isLastLineVolatile) {
        stream << "\r\033[K" << std::flush;
    }
    isLastLineVolatile = false;
}

void StdoutLoggingExtension::printTestStatus(const Test& test) {
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

void StdoutLoggingExtension::printTestExecutionTime(const Test& test) {
    if (test.getAvgTimeTicksForExecution() != -1.0) {
        stream << std::fixed << std::setprecision(3)
               << (test.getNumAttempts() > 1 ? "~ " : "")
               << test.getAvgTimeTicksForExecution() << " ticks ("
               << (test.getNumAttempts() > 1 ? "~" : "")
               << static_cast<double>(
                    TimeTicksToNanoseconds(test.getAvgTimeTicksForExecution())
                      .count())
            * std::milli::den / std::nano::den
               << " ms)";
    } else {
        stream << "(unknown time)";
    }
}

void StdoutLoggingExtension::printTestAttemptsInfo(const Test& test) {
    stream << "(passed: " << test.getNumPassedAttempts() << "/"
           << test.getNumAttempts();
    if (!test.isPassed()) {
        stream << ", required: " << test.getNumRequiredPassedAttempts();
    }
    stream << ")";
}

void StdoutLoggingExtension::printTestFailure(const Test::ExecutionInfo& info) {
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

void StdoutLoggingExtension::printTestMessage(const Test& test) {
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

void StdoutLoggingExtension::updateVolatileLine(const Test& test) {
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
