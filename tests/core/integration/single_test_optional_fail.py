#! /usr/bin/env python3.7

import subprocess

proc = None
try:
    proc = subprocess.run("./core_single_test_optional_fail",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

if proc.returncode != 0:
    print("Did not exit with code 0, exit_code=" + str(proc.returncode))
    exit(1)

if "[F] TestCase::test" not in proc.stdout.decode():
    print("Did not output correct feedback on failing test, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "[F] TestCase::optional-group::non-optional-test" not in proc.stdout.decode():
    print("Did not output correct feedback on failing test, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests passed: 0" not in proc.stdout.decode():
    print("Did not output correct feedback on number of passed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests failed: 2 (2 were optional)" not in proc.stdout.decode():
    print("Did not output correct feedback on number of failed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)
