#! /usr/bin/env python3

import subprocess

proc = None
try:
    proc = subprocess.run("./multiple_tests_one_fails",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

if proc.returncode != 1:
    print("Did not exit with code 1, exit_code=" + str(proc.returncode))
    exit(1)

if "[P] Multiple tests::test1" not in str(proc.stdout):
    print("Did not output correct feedback on passing test 1, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "[F] Multiple tests::test2" not in str(proc.stdout):
    print("Did not output correct feedback on failing test 2, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "[P] Multiple tests::test3" not in str(proc.stdout):
    print("Did not output correct feedback on passing test 3, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "[P] Multiple tests::test4" not in str(proc.stdout):
    print("Did not output correct feedback on passing test 4, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "Tests passed: 3" not in str(proc.stdout):
    print("Did not output correct feedback on number of passed tests, output='"
          + str(proc.stdout) + "'")
    exit(1)

if "Tests failed: 1" not in str(proc.stdout):
    print("Did not output correct feedback on number of failed tests, output='"
          + str(proc.stdout) + "'")
    exit(1)
