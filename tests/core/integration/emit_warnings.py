#! /usr/bin/env python3.7
import re
import subprocess
import sys

proc = None
try:
    proc = subprocess.run(["./core_emit_warnings", "--executor=" + sys.argv[1]],
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

output = proc.stdout.decode("ascii")


def check_warning(message_pattern, line=0, notes=None):
    pattern = r"Warning\: " + message_pattern + r"\n"
    if line != 0:
        pattern += r"\tat (.*)tests/core/integration/emit_warnings\.cpp\:" + str(
            line) + r"\:[0-9]*\n"
    for note in (notes or []):
        pattern += r"\tNote\: " + note[0]
        if note[1] != 0:
            pattern += r" at (.*)tests/core/integration/emit_warnings\.cpp\:" + str(
                note[1]) + r"\:[0-9]*"
        pattern += r"\n"
    if not re.search(pattern, output):
        print("Test did not emit expected warning with pattern: \"\"\"" + pattern + "\"\"\"")
        print("Output:")
        print(proc.stdout.decode("ascii"))
        exit(1)


group_test_case_note = ("In group TestCase", 7)
group1_note = ("In group group", 11)
group2_note = ("In group group2", 61)

check_warning(r"Called fail\(\) outside a test, ignoring\.", 9,
              [group_test_case_note])
check_warning(r"Called fail\(\) outside a test, ignoring\.", 13,
              [group1_note, group_test_case_note])

check_warning(
    r"Exception thrown outside a test: exception-in-group",
    notes=[(r"Unable to execute remainder of tests in this group\.", 0), group1_note,
           group_test_case_note])

check_warning(
    r"Non-exception object thrown outside a test\.",
    notes=[(r"Unable to execute remainder of tests in this group\.", 0), group2_note,
           group_test_case_note])

set_up_note = ("While running setUp", 15)
test_note = ("While running test test", 43)
notes_inside_set_up = [set_up_note, test_note, group1_note, group_test_case_note]
check_warning(r"Called setUp\(\) inside a setUp\(\), ignoring\.", 17, notes_inside_set_up)
check_warning(r"Called tearDown\(\) inside a setUp\(\), ignoring\.", 20, notes_inside_set_up)
check_warning(r"Called test\(\) inside a setUp\(\), ignoring\.", 23, notes_inside_set_up)
check_warning(r"Called group\(\) inside a setUp\(\), ignoring\.", 26, notes_inside_set_up)

notes_inside_test = [test_note, group1_note, group_test_case_note]
check_warning(r"Called setUp\(\) inside a test\(\), ignoring\.", 45, notes_inside_test)
check_warning(r"Called tearDown\(\) inside a test\(\), ignoring\.", 48, notes_inside_test)
check_warning(r"Called test\(\) inside a test\(\), ignoring\.", 51, notes_inside_test)
check_warning(r"Called group\(\) inside a test\(\), ignoring\.", 54, notes_inside_test)

tear_down_note = ("While running tearDown", 29)
notes_inside_teardown = [tear_down_note, test_note, group1_note, group_test_case_note]
check_warning(r"Called setUp\(\) inside a tearDown\(\), ignoring\.", 31, notes_inside_teardown)
check_warning(r"Called tearDown\(\) inside a tearDown\(\), ignoring\.", 34, notes_inside_teardown)
check_warning(r"Called test\(\) inside a tearDown\(\), ignoring\.", 37, notes_inside_teardown)
check_warning(r"Called group\(\) inside a tearDown\(\), ignoring\.", 40, notes_inside_teardown)

check_warning(r"Called test\(\) from a different thread than the main testing thread, ignoring\.",
              68, [group_test_case_note])
check_warning(r"Called group\(\) from a different thread than the main testing thread, ignoring\.",
              71, [group_test_case_note])
check_warning(r"Called setUp\(\) from a different thread than the main testing thread, ignoring\.",
              75, [group_test_case_note])
check_warning(
    r"Called tearDown\(\) from a different thread than the main testing thread, ignoring\.", 78,
    [group_test_case_note])

if 'test-in-setUp' in proc.stdout.decode():
    print('test-in-setUp was executed!')
    exit(1)
if 'test-in-test' in proc.stdout.decode():
    print('test-in-test was executed!')
    exit(1)
if 'test-in-tearDown' in proc.stdout.decode():
    print('test-in-tearDown was executed!')
    exit(1)
if 'test-in-thread' in proc.stdout.decode():
    print('test-in-thread was executed!')
    exit(1)
if 'test-in-group-in-thread' in proc.stdout.decode():
    print('test-in-group-in-thread was executed!')
    exit(1)
