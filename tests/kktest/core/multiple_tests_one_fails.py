#! /usr/bin/env python3

import subprocess

proc = None
try:
    proc = subprocess.run("./core_multiple_tests_one_fails",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

if proc.returncode != 1:
    print("Did not exit with code 1, exit_code=" + str(proc.returncode))
    exit(1)

if "[P] TestCase::test1" not in proc.stdout.decode():
    print("Did not output correct feedback on passing test 1, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "[F] TestCase::test2" not in proc.stdout.decode():
    print("Did not output correct feedback on failing test 2, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "[P] TestCase::test3" not in proc.stdout.decode():
    print("Did not output correct feedback on passing test 3, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "[P] TestCase::test4" not in proc.stdout.decode():
    print("Did not output correct feedback on passing test 4, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests passed: 3" not in proc.stdout.decode():
    print("Did not output correct feedback on number of passed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)

if "Tests failed: 1" not in proc.stdout.decode():
    print("Did not output correct feedback on number of failed tests, output='"
          + proc.stdout.decode() + "'")
    exit(1)
