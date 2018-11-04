#!/usr/bin/env python3

TEST_CASES = [
    "comparison_matcher_test",
    "truth_matcher_test",
    "numeric_matchers_test",
]


def run(cmd):
    import subprocess
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    while process.poll() is None:
        chars = process.stdout.readline()
        if len(chars) > 0:
            print(str(chars, encoding='utf-8'), end='')
    if process.returncode != 0:
        exit(process.returncode)


run("./compile_dev.sh")

for test in TEST_CASES:
    run(["./build/tests/{}".format(test)])
