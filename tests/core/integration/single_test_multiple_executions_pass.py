#! /usr/bin/env python3.7

import subprocess
import sys

proc = None
try:
    proc = subprocess.run(
        ["./core_single_test_multiple_executions_pass", "--executor=" + sys.argv[1]],
        timeout=10, capture_output=True)
except TimeoutError:
    print("Test did not finish in 10 seconds")
    exit(1)

if proc.returncode != 0:
    print("Did not exit with code 0, exit_code=" + str(proc.returncode))
    exit(1)

if "[P] TestCase::test" not in proc.stdout.decode():
    print("Did not output correct feedback on passing test, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests passed: 1" not in proc.stdout.decode():
    print("Did not output correct feedback on number of passed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests failed: 0" not in proc.stdout.decode():
    print("Did not output correct feedback on number of failed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)
