#!/usr/bin/env python3
# Script to easily generate new CTest Units

import shutil

PREFIX = "test"
BASE_C_FILE = "test_base"
TESTS_FOLDER = "tests"
CMAKELISTS = "CMakeLists.txt"

test_name = input("Enter test name: ")


def append_line(path, line):
    with open(path, "a+") as f:
        f.write("\n")
        f.write(line)


def add_test_to_cmakelists(new_test_name):
    append_line(CMAKELISTS, f"add_executable({PREFIX}_{test_name} {TESTS_FOLDER}/{PREFIX}_{test_name}.c)")
    append_line(CMAKELISTS, f"add_test(Test-{test_name} {PREFIX}_{test_name})")


def create_test_file(new_test_name):
    original_path = f"{TESTS_FOLDER}/{BASE_C_FILE}.c"
    target_path = f"{TESTS_FOLDER}/{PREFIX}_{new_test_name}.c"
    shutil.copyfile(original_path, target_path)
    add_test_to_cmakelists(new_test_name)


print(f"Added new test {PREFIX}-{test_name} to {CMAKELISTS}!")

create_test_file(test_name)
