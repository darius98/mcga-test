#! /usr/bin/env python3

import subprocess

proc = None
try:
    proc = subprocess.run("./single_failing_test",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

if proc.returncode != 1:
    print("Did not exit with code 1, exit_code=" + str(proc.returncode))
    exit(1)

if "[F] Single failing test::This test fails" not in str(proc.stdout):
    print("Did not output correct feedback on failing test, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "Tests passed: 0" not in str(proc.stdout):
    print("Did not output correct feedback on number of passed tests, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "Tests failed: 1" not in str(proc.stdout):
    print("Did not output correct feedback on number of failed tests, output='"
          + str(proc.stdout) + "'")
    exit(1)
