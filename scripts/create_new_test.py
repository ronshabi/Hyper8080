#!/usr/bin/env python3
# Script to easily generate new CTest Units

import shutil
import sys

PREFIX = "test"
BASE_C_FILE = "test_base"
TESTS_FOLDER = "tests"
CMAKELISTS = "CMakeLists.txt"


def append_line(path, line):
    with open(path, "a+") as f:
        f.write("\n")
        f.write(line)


def add_test_to_cmakelists(new_test_name):
    append_line(CMAKELISTS, f"add_executable({PREFIX}_{new_test_name} {TESTS_FOLDER}/{PREFIX}_{test_name}.c)")
    append_line(CMAKELISTS, f"add_test(Test-{new_test_name} {PREFIX}_{test_name})")
    append_line(CMAKELISTS, '\n')
    print(f"Added new test {PREFIX}-{test_name} to {CMAKELISTS}!")


def create_test_file(new_test_name):
    try:
        original_path = f"{TESTS_FOLDER}/{BASE_C_FILE}.c"
        target_path = f"{TESTS_FOLDER}/{PREFIX}_{new_test_name}.c"
        shutil.copyfile(original_path, target_path)
        add_test_to_cmakelists(new_test_name)
        print(f"Test file created at {TESTS_FOLDER}/{PREFIX}_{new_test_name}.c")
    except:
        print(f"Usage: run this script from the project's root directory.")

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Easily generate new CTests")
        print(f"Usage: python3 {str(sys.argv[0])} <test_name>")
        print("Run this from the project's root directory");
        exit(1)

    test_name = str(sys.argv[1])
    create_test_file(test_name)
