#! /usr/bin/env python3

import subprocess

proc = None
try:
    proc = subprocess.run("./core_single_test_exit_0",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

if proc.returncode != 1:
    print("Test did not exit with code 1, exit_code=" + str(proc.returncode))
    exit(1)

if "[F] TestCase::test - (unknown time)\n" \
   "\tUnexpected 0-code exit." not in proc.stdout.decode():
    print("Test did not correctly report exit of unexpected 0-exit test")
    print("Output:")
    print(proc.stdout.decode())
    exit(1)
