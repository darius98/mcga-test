#! /usr/bin/env python3.7

import subprocess

proc = None
try:
    proc = subprocess.run("./core_emit_warnings",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)


def check_warning(message):
    if "Warning: " + message + "\n" not in proc.stdout.decode("ascii"):
        print("Test did not emit warning '" + message + "'")
        print("Output:")
        print(proc.stdout.decode("ascii"))
        exit(1)


check_warning("Called fail() with message 'fail-top-level' outside a test execution. Ignoring.")
check_warning("Called fail() with message 'fail-in-group' outside a test execution. Ignoring.")
check_warning("Exception thrown in group \"group\": exception-in-group. Unable to execute remainder of tests in this group.")
check_warning("Non-exception thrown in group \"group2\". Unable to execute remainder of tests in this group.")
check_warning("Called setUp() inside a setUp(). Ignoring.")
check_warning("Called tearDown() inside a setUp(). Ignoring.")
check_warning("Called test() inside a setUp(). Ignoring.")
check_warning("Called group() inside a setUp(). Ignoring.")
check_warning("Called setUp() inside a test(). Ignoring.")
check_warning("Called tearDown() inside a test(). Ignoring.")
check_warning("Called test() inside a test(). Ignoring.")
check_warning("Called group() inside a test(). Ignoring.")
check_warning("Called setUp() inside a tearDown(). Ignoring.")
check_warning("Called tearDown() inside a tearDown(). Ignoring.")
check_warning("Called test() inside a tearDown(). Ignoring.")
check_warning("Called group() inside a tearDown(). Ignoring.")
check_warning("Called test() from a different thread than the main testing thread. Ignoring.")
check_warning("Called group() from a different thread than the main testing thread. Ignoring.")
check_warning("Called setUp() from a different thread than the main testing thread. Ignoring.")
check_warning("Called tearDown() from a different thread than the main testing thread. Ignoring.")

# Tests 'test-in-test', 'test-in-setUp' and 'test-in-tearDown'
# should not have been executed.

if 'test-in-setUp' in proc.stdout.decode():
    print('test-in-setUp was executed!')
    exit(1)
if 'test-in-test' in proc.stdout.decode():
    print('test-in-test was executed!')
    exit(1)
if 'test-in-tearDown' in proc.stdout.decode():
    print('test-in-tearDown was executed!')
    exit(1)
