#! /usr/bin/env python3.7

import re
import subprocess
import unittest
from typing import List, Optional


class MCGATestIntegrationMixin(unittest.TestCase):
    executor_type: str

    _last_test_args: Optional[List[str]] = None
    _last_test_output: Optional[str] = None

    @staticmethod
    def output_test_line(status, name, approx=False, approx_fail=False):
        if not approx:
            return "\\[" + status + "] " + name + " - [0-9.]* ticks \\([0-9.]* ms\\)\n"
        approx_tick_pattern = " - \\~ [0-9.]* ticks \\(\\~[0-9.]* ms\\) "
        if not approx_fail:
            return "\\[" + status + "] " + name + approx_tick_pattern + "\\(passed: [0-9]*/[0-9]*\\)\n"
        return "\\[" + status + "] " + name + approx_tick_pattern + "\\(passed: [0-9]*/[0-9]*\\, required: [0-9]*\\)\n"

    @staticmethod
    def output_test_pattern(status, name, approx=False, approx_fail=False):
        return re.compile(
            MCGATestIntegrationMixin.output_test_line(status, name, approx, approx_fail))

    def run_test_bin(self, name=None, timeout=1, expect_fail=False, tests_passed=0, tests_failed=0,
                     tests_failed_optional=None, tests_skipped=None, expect_output_contains=None,
                     expect_output_not_contains=None, extra_args=None):
        if name is None:
            name = self._testMethodName[5:]
        test_args = ["./" + name, "--executor=" + self.executor_type]
        if extra_args is not None:
            test_args += extra_args
        try:
            proc = subprocess.run(test_args, timeout=timeout, capture_output=True)
        except TimeoutError:
            self.fail("Test execution timed out ({} second{})".format(timeout,
                                                                      "s" if timeout != 1 else ""))
        output = proc.stdout.decode()
        self._last_test_args = test_args
        self._last_test_output = output
        self.assertEqual(proc.returncode != 0, expect_fail,
                         "Expected process to exit with code {}, but exited with {} instead. "
                         "Output was: \"\"\"{}\"\"\"".format(
                             1 if expect_fail else 0, proc.returncode, output))
        summary_snippet = "Tests passed: " + str(tests_passed) + "\nTests failed: " + str(
            tests_failed)
        if tests_failed_optional is not None:
            summary_snippet += " \\(" + str(tests_failed_optional) + " were optional\\)"
        if tests_skipped is not None:
            summary_snippet += "\nTests skipped: " + str(tests_skipped)
        summary_snippet += "\nTotal recorded testing time: [0-9\\.]* ticks \\([0-9\\.]* ms\\)\n"
        self.check_output(re.compile(summary_snippet))
        if expect_output_contains is not None:
            self.check_output(expect_output_contains)
        if expect_output_not_contains is not None:
            self.check_output(expect_output_not_contains, expect_contains=False)
        return output

    def check_output(self, snippet, msg=None, expect_contains=True):
        if isinstance(snippet, list):
            for entry in snippet:
                self.check_output(entry, msg, expect_contains)
            return

        msg = "Test executable command: " + " ".join(self._last_test_args) + "\n"
        msg += "Output {} {}: \"\"\"{}\"\"\"\nOutput was: \"\"\"{}\"\"\"".format(
            "did not contain expected" if expect_contains else "contained unexpected",
            "pattern" if isinstance(snippet, re.Pattern) else "snippet",
            snippet.pattern if isinstance(snippet, re.Pattern) else snippet,
            self._last_test_output)
        if isinstance(snippet, re.Pattern):
            self.assertEqual(bool(snippet.search(self._last_test_output)), expect_contains, msg)
        else:
            self.assertEqual(bool(snippet in self._last_test_output), expect_contains, msg)

    def check_warning(self, message_pattern, line=0, notes=None):
        pattern = r"Warning\: " + message_pattern + r"\n"
        if line != 0:
            pattern += r"\tat (.*)tests/integration/emit_warnings\.cpp\:" + str(
                line) + r"\:[0-9]*\n"
        for note in (notes or []):
            pattern += r"\tNote\: " + note[0]
            if note[1] != 0:
                pattern += r" at (.*)tests/integration/emit_warnings\.cpp\:" + str(
                    note[1]) + r"\:[0-9]*"
            pattern += r"\n"
        self.check_output(re.compile(pattern))


class MCGATestIntegrationSmoothTestCase(MCGATestIntegrationMixin):
    executor_type = "smooth"

    def test_emit_warnings(self):
        self.run_test_bin(expect_fail=True, tests_passed=1, tests_failed=0,
                          expect_output_not_contains=[
                              "test-in-setUp",
                              "test-in-test",
                              "test-in-tearDown",
                              "test-in-thread",
                              "test-in-group-in-thread",
                          ])

        group_test_case_note = ("In group TestCase", 7)
        group1_note = ("In group group", 17)
        group2_note = ("In group group2", 73)

        self.check_warning(r"Called fail\(\) outside a test, ignoring\.", 9,
                           [group_test_case_note])
        self.check_warning(r"Called cleanup\(\) outside a test, ignoring\.", 12,
                           [group_test_case_note])
        self.check_warning(r"Called skip\(\) outside a test, ignoring\.", 15,
                           [group_test_case_note])
        self.check_warning(r"Called fail\(\) outside a test, ignoring\.", 19,
                           [group1_note, group_test_case_note])
        self.check_warning(r"Called cleanup\(\) outside a test, ignoring\.", 22,
                           [group1_note, group_test_case_note])
        self.check_warning(r"Called skip\(\) outside a test, ignoring\.", 25,
                           [group1_note, group_test_case_note])

        self.check_warning(
            r"Exception thrown outside a test: exception-in-group",
            notes=[(r"Unable to execute remainder of tests in this group\.", 0), group1_note,
                   group_test_case_note])

        self.check_warning(
            r"Non-exception object thrown outside a test\.",
            notes=[(r"Unable to execute remainder of tests in this group\.", 0), group2_note,
                   group_test_case_note])

        set_up_note = ("While running setUp", 27)
        test_note = ("While running test test", 55)
        notes_inside_set_up = [set_up_note, test_note, group1_note, group_test_case_note]
        self.check_warning(r"Called setUp\(\) inside a setUp\(\), ignoring\.", 29,
                           notes_inside_set_up)
        self.check_warning(r"Called tearDown\(\) inside a setUp\(\), ignoring\.", 32,
                           notes_inside_set_up)
        self.check_warning(r"Called test\(\) inside a setUp\(\), ignoring\.", 35,
                           notes_inside_set_up)
        self.check_warning(r"Called group\(\) inside a setUp\(\), ignoring\.", 38,
                           notes_inside_set_up)

        notes_inside_test = [test_note, group1_note, group_test_case_note]
        self.check_warning(r"Called setUp\(\) inside a test\(\), ignoring\.", 57, notes_inside_test)
        self.check_warning(r"Called tearDown\(\) inside a test\(\), ignoring\.", 60,
                           notes_inside_test)
        self.check_warning(r"Called test\(\) inside a test\(\), ignoring\.", 63, notes_inside_test)
        self.check_warning(r"Called group\(\) inside a test\(\), ignoring\.", 66, notes_inside_test)

        tear_down_note = ("While running tearDown", 41)
        notes_inside_teardown = [tear_down_note, test_note, group1_note, group_test_case_note]
        self.check_warning(r"Called setUp\(\) inside a tearDown\(\), ignoring\.", 43,
                           notes_inside_teardown)
        self.check_warning(r"Called tearDown\(\) inside a tearDown\(\), ignoring\.", 46,
                           notes_inside_teardown)
        self.check_warning(r"Called test\(\) inside a tearDown\(\), ignoring\.", 49,
                           notes_inside_teardown)
        self.check_warning(r"Called group\(\) inside a tearDown\(\), ignoring\.", 52,
                           notes_inside_teardown)

        self.check_warning(
            r"Called test\(\) from a different thread than the main testing thread, ignoring\.",
            80, [group_test_case_note])
        self.check_warning(
            r"Called group\(\) from a different thread than the main testing thread, ignoring\.",
            83, [group_test_case_note])
        self.check_warning(
            r"Called setUp\(\) from a different thread than the main testing thread, ignoring\.",
            88, [group_test_case_note])
        self.check_warning(
            r"Called tearDown\(\) from a different thread than the main testing thread, ignoring\.",
            91,
            [group_test_case_note])

    def test_multiple_files(self):
        self.run_test_bin(tests_passed=2, tests_failed=0, expect_output_contains=[
            self.output_test_pattern("P", "TestCase1::test"),
            self.output_test_pattern("P", "TestCase2::test"),
        ])

    def test_multiple_tests_one_fails(self):
        self.run_test_bin(expect_fail=True, tests_passed=3, tests_failed=1, expect_output_contains=[
            self.output_test_pattern("P", "TestCase::test1"),
            self.output_test_pattern("F", "TestCase::test2"),
            self.output_test_pattern("P", "TestCase::test3"),
            self.output_test_pattern("P", "TestCase::test4"),
        ])

    def test_single_file_multiple_cases(self):
        self.run_test_bin(tests_passed=2, tests_failed=0, expect_output_contains=[
            self.output_test_pattern("P", "TestCase1::test"),
            self.output_test_pattern("P", "TestCase2::test"),
        ])

    def test_fail(self):
        self.run_test_bin(expect_fail=True, tests_passed=0, tests_failed=1, expect_output_contains=[
            self.output_test_pattern("F", "TestCase::test"),
        ])

    def test_fail_in_thread(self):
        self.run_test_bin(expect_fail=True, tests_passed=0, tests_failed=1, expect_output_contains=[
            self.output_test_pattern("F", "TestCase::test"),
        ])

    def test_skipped_test(self):
        # Skipped tests are not printed by default.
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                          ],
                          expect_output_not_contains=[
                              "TestCase::test-2",
                              "Skipped at",
                              "skip-this-test",
                          ])
        output_pattern = re.compile(
            self.output_test_line("S", "TestCase::test-2") +
            "Skipped at .*/tests/integration/skipped_test\\.cpp:7:9\n"
            "\tskip-this-test")
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped"],
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                              output_pattern,
                          ])
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped", "--fail-on-skip"], expect_fail=True,
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                              output_pattern,
                          ])

    def test_skipped_test_thread(self):
        # Skipped tests are not printed by default.
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                          ],
                          expect_output_not_contains=[
                              "TestCase::test-2",
                              "Skipped at",
                              "In a different thread",
                          ])
        output_pattern = re.compile(
            self.output_test_line("S", "TestCase::test-2") +
            "Skipped at .*/tests/integration/skipped_test_thread\\.cpp:10:13\n"
            "\tIn a different thread")
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped"],
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                              output_pattern,
                          ])
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped", "--fail-on-skip"], expect_fail=True,
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                              output_pattern,
                          ])

    def test_multiple_executions_fail(self):
        self.run_test_bin(expect_fail=True, timeout=10, tests_passed=0, tests_failed=1,
                          expect_output_contains=[
                              self.output_test_pattern("F", "TestCase::test", approx=True,
                                                       approx_fail=True),
                          ])

    def test_multiple_executions_pass(self):
        self.run_test_bin(timeout=10, tests_passed=1, tests_failed=0, expect_output_contains=[
            self.output_test_pattern("P", "TestCase::test", approx=True),
        ])

    def test_multiple_executions_skip(self):
        # Skipped tests are not printed by default.
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1, timeout=10,
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                          ],
                          expect_output_not_contains=[
                              "TestCase::test-2",
                              "Skipped at",
                              "every-time",
                          ])
        output_pattern = re.compile(
            self.output_test_line("S", "TestCase::test-2", approx=True, approx_fail=True) +
            "Skipped at .*/tests/integration/multiple_executions_skip\\.cpp:10:26\n"
            "\tevery-time")
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1, timeout=10,
                          extra_args=["--print-skipped"],
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                              output_pattern,
                          ])
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1, timeout=10,
                          extra_args=["--print-skipped", "--fail-on-skip"], expect_fail=True,
                          expect_output_contains=output_pattern)

    def test_optional_fail(self):
        self.run_test_bin(tests_passed=0, tests_failed=2, tests_failed_optional=2,
                          expect_output_contains=[
                              self.output_test_pattern("F", "TestCase::test"),
                              self.output_test_pattern("F", "TestCase::opt-group::non-opt-test"),
                          ])

    def test_pass(self):
        self.run_test_bin(tests_passed=1, tests_failed=0, expect_output_contains=[
            self.output_test_pattern("P", "TestCase::test"),
        ])

    def test_set_up_tear_down_order(self):
        self.run_test_bin(tests_passed=2, tests_failed=0, expect_output_contains=[
            re.compile("first-set-up\n"
                       "second-set-up\n"
                       "third-set-up\n"
                       "first-test\n"
                       "set-up-cleanup\n"
                       "first-cleanup\n"
                       "second-cleanup\n"
                       "third-tear-down\n"
                       "second-tear-down\n"
                       "first-tear-down\n"
                       + self.output_test_line("P", "TestCase::test1") +
                       "first-set-up\n"
                       "second-set-up\n"
                       "third-set-up\n"
                       "second-test\n"
                       "set-up-cleanup\n"
                       "third-tear-down\n"
                       "second-tear-down\n"
                       "first-tear-down\n"
                       + self.output_test_line("P", "TestCase::test2")),
        ])

    def test_fail_in_set_up(self):
        self.run_test_bin(tests_passed=0, tests_failed=1, expect_fail=True, expect_output_contains=[
            re.compile("first-set-up\n"
                       "second-set-up\n"
                       "third-tear-down\n"
                       "second-tear-down\n"
                       "first-tear-down\n"
                       + self.output_test_line("F", "TestCase::test") +
                       "Failed at .*tests/integration/fail_in_set_up\\.cpp:17:13\n"
                       "\tfail-in-second-setup\n")
        ], expect_output_not_contains=[
            "third-set-up",
            "fourth-set-up",
            "fourth-tear-down",
            "test-body",
        ])

    def test_fail_in_tear_down(self):
        self.run_test_bin(tests_passed=0, tests_failed=1, expect_fail=True, expect_output_contains=[
            re.compile("first-set-up\n"
                       "second-set-up\n"
                       "third-set-up\n"
                       "fourth-set-up\n"
                       "test-body\n"
                       "fourth-tear-down\n"
                       "third-tear-down\n"
                       "second-tear-down\n"
                       "first-tear-down\n"
                       + self.output_test_line("F", "TestCase::test") +
                       "Failed at .*tests/integration/fail_in_tear_down\\.cpp:25:13\n"
                       "\tfail-in-third-teardown\n"),
        ])

    def test_fail_in_cleanup(self):
        self.run_test_bin(tests_passed=0, tests_failed=1, expect_fail=True, expect_output_contains=[
            re.compile("set-up\n"
                       "test-body\n"
                       "cleanup-in-set-up\n"
                       "second-tear-down\n"
                       "first-tear-down\n"
                       + self.output_test_line("F", "TestCase::group::test") +
                       "Failed at .*tests/integration/fail_in_cleanup\\.cpp:10:13\n"
                       "\tfail-in-cleanup\n"),
        ])

    def test_skip_in_set_up(self):
        # Skipped tests are not printed by default.
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                              "set-up\ntear-down\n",
                          ],
                          expect_output_not_contains=[
                              "TestCase::test-2",
                              "Skipped at",
                              "skip-in-set-up",
                              "test-body",
                              "fail-in-test-body",
                          ])
        output_pattern = re.compile(
            self.output_test_line("P", "TestCase::test-1") +
            "set-up\n"
            "tear-down\n"
            + self.output_test_line("S", "TestCase::test-2") +
            "Skipped at .*/tests/integration/skip_in_set_up\\.cpp:11:13\n"
            "\tskip-in-set-up")
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped"],
                          expect_output_contains=output_pattern,
                          expect_output_not_contains=["test-body", "fail-in-test-body"])
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped", "--fail-on-skip"], expect_fail=True,
                          expect_output_contains=output_pattern,
                          expect_output_not_contains=["test-body", "fail-in-test-body"])

    def test_skip_in_tear_down(self):
        # Skipped tests are not printed by default.
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          expect_output_contains=[
                              self.output_test_pattern("P", "TestCase::test-1"),
                          ],
                          expect_output_not_contains=[
                              "TestCase::test-2",
                              "Skipped at",
                              "skip-in-tear-down",
                              "fail-in-test-body",
                          ])
        output_pattern = re.compile(
            self.output_test_line("P", "TestCase::test-1") +
            self.output_test_line("S", "TestCase::test-2") +
            "Skipped at .*/tests/integration/skip_in_tear_down\\.cpp:10:13\n"
            "\tskip-in-tear-down")
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped"],
                          expect_output_contains=output_pattern,
                          expect_output_not_contains="fail-in-test-body")
        self.run_test_bin(tests_passed=1, tests_failed=0, tests_skipped=1,
                          extra_args=["--print-skipped", "--fail-on-skip"], expect_fail=True,
                          expect_output_contains=output_pattern,
                          expect_output_not_contains="fail-in-test-body")


class MCGATestIntegrationBoxedTestCase(MCGATestIntegrationSmoothTestCase):
    executor_type = "boxed"

    def test_exit_0(self):
        self.run_test_bin(expect_fail=True, tests_passed=0, tests_failed=1, expect_output_contains=[
            self.output_test_pattern("F", "TestCase::test"),
            "\tUnexpected 0-code exit.",
        ])

    def test_exit_1(self):
        self.run_test_bin(expect_fail=True, tests_passed=0, tests_failed=1, expect_output_contains=[
            self.output_test_pattern("F", "TestCase::test"),
            "\tTest exited with code 1",
        ])

    def test_kbs(self):
        self.run_test_bin(expect_fail=True, tests_passed=0, tests_failed=1, expect_output_contains=[
            self.output_test_pattern("F", "TestCase::test"),
            "\tTest killed by signal 15",
        ])

    def test_timeout(self):
        self.run_test_bin(expect_fail=True, timeout=10, tests_passed=0, tests_failed=1,
                          expect_output_contains=[
                              self.output_test_pattern("F", "TestCase::test"),
                              "\tTest execution timed out.",
                          ])


if __name__ == "__main__":
    unittest.main(verbosity=2)
