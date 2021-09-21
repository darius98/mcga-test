#! /usr/bin/env python3.7

import subprocess
import sys

proc = None
try:
    proc = subprocess.run(
        ["./core_single_test_multiple_executions_fail", "--executor=" + sys.argv[1]],
        timeout=10, capture_output=True)
except TimeoutError:
    print("Test did not finish in 10 seconds")
    exit(1)

if proc.returncode != 1:
    print("Did not exit with code 1, exit_code=" + str(proc.returncode))
    exit(1)

if "[F] TestCase::test" not in proc.stdout.decode():
    print("Did not output correct feedback on failing test, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests passed: 0" not in proc.stdout.decode():
    print("Did not output correct feedback on number of passed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests failed: 1" not in proc.stdout.decode():
    print("Did not output correct feedback on number of failed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)
