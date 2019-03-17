#! /usr/bin/env python3

import subprocess

proc = None
try:
    proc = subprocess.run("./core_single_test_exit_1",
                          timeout=1, capture_output=True)
except TimeoutError:
    print("Test did not finish in 1 second")
    exit(1)

if proc.returncode != 1:
    print("Test did not exit with code 1, exit_code=" + str(proc.returncode))
    exit(1)

if "[F] Single test exit 1::This test exits with code 1 - (unknown time)\n" \
   "\tTest exited with code 1" not in proc.stdout.decode():
    print("Test did not correctly report exit of unexpected 1-exit test")
    print("Output:")
    print(proc.stdout.decode())
    exit(1)
