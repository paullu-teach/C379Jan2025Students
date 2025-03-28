# Modified by Paul Lu, 2025
#   Original verion Copyright 2020 Logan Gilmour, source logan/grading_tools
# Modified by Xinlei Chen
#	Significant changes hacked for GitHub classroom.  TODO More cleanup.


import os
import zipfile
import tarfile
# import shutil
# from contextlib import contextmanager
import argparse

conf = {
    "submission_name": "assn02",
    "specified_files_c": [ "assn02/distpsnotify.c", "assn02/remoteagent.c", "assn02/Makefile"],
    "specified_files_cc": [ "assn02/distpsnotify.cc", "assn02/remoteagent.cc", "assn02/Makefile"],
    "assignment_version": "Assignment #2 2025",
}

# Written with help from Copilot
# Determine what kind of archive, and get list of files from it
def get_contents_archive(path):
    # print(path) # Debug
    if path.lower().endswith(".zip"):
        with zipfile.ZipFile(path, "r") as zip_ref:
            return zip_ref.namelist()
    elif path.lower().endswith(".tar"):
        with tarfile.open(path, "r") as tar_ref:
            return tar_ref.getnames()
    elif path.lower().endswith(".tar.gz"):
        with tarfile.open(path, "r:gz") as tar_ref:
            return tar_ref.getnames()
    else:
        print("*** Error:  Incorrect archive, file extension")
        exit(1)

# def validate_contents(path, all_files, specified_files, all_conf):
def validate_contents(path, all_files, all_conf):
    found_files = []
    okextra_files = []
    notokextra_files = []
    for ff in all_files:
        f = ff.strip("/")
        if all_conf["submission_name"] == f:
            okextra_files.append(f)
            continue
        if ".git" in f:
            okextra_files.append(f)
            continue
        if ".DS_Store" in f:
            okextra_files.append(f)
            continue
        if "__pycache__" in f:
            notokextra_files.append(f)
            continue
        if ".swp" in f:
            okextra_files.append(f)
            continue
        if "Tests" in f:
            okextra_files.append(f)
            continue
        if "Expected" in f:
            okextra_files.append(f)
            continue
        if "Demo" in f:
            okextra_files.append(f)
            continue
        if f == "grade.txt":
            okextra_files.append(f)
            continue
        found_files.append(f)

    # Deal with "one of" .c or .cc
    # if(".c" in found_files and ".cc" not in found_files):
    # elif(".cc" in found_files and ".c" not in found_files):

    # Hack:  Check .cc first, because .c matches both .cc and .c
    # https://stackoverflow.com/questions/4843158/how-to-check-if-a-string-is-a-substring-of-items-in-a-list-of-strings
    if any(".cc" in s for s in found_files):
        specified_files = all_conf["specified_files_cc"]
    elif any(".c" in s for s in found_files):
        specified_files = all_conf["specified_files_c"]
    else:
        print("*** C/C++ files error")
        exit(-1)

    specified_files.sort()
    found_files.sort()
    okextra_files.sort()
    notokextra_files.sort()

    # print("Files found in {}:".format(path))
    # print("\n".join(found_files))

    # print("\n".join(specified_files))  # Debug
    # print("\n".join(extra_files))  # Debug

    if set(found_files) != set(specified_files):
        print("\n{} should contain:\n{}\n\nbut instead contains:\n\n{}"
              .format(path, "\n".join(specified_files), "\n".join(found_files)))
        return False 
    else:
        print("\n{} contains:\n\n{}\n"
              .format(path, "\n".join(found_files)))
        return True 


def validate_submission(path):
    print("=== CMPUT 379 {} Validator ===".format(conf["assignment_version"]))

    names_list = get_contents_archive(path)
    ret = validate_contents(path, names_list, conf)
    if(ret == True):
        print("File structure appears to be correct.")
        print("\nVALIDATION SUCCEEDED.")
        print("\nDISCLAIMER: This does not mean that your submission is correct "
              "- just that it appears to have the right files and apparent structure.")
        return True
    else:
        print("\nVALIDATION FAILED")
        print("Stopping validation. Please fix this and try again.")
        return False

def stuff():
    try:
        pass
    except ValidationException as e:
        print("\nVALIDATION FAILED")
        print(e)
        print("Please fix this and try validating again.")
        return False

    except Exception as e:
        print("\nVALIDATION FAILED")
        print("Exception occurred: {}".format(e))
        print("Stopping validation. Please fix this and try again.")
        return False


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
                    description='''This script helps you to verify that your submission structure is correct for {}.
Usage:
  1. Run the program using:
        python3 submission_validator.py <path to archive file>
       E.g.,
        python3 submission_validator.py ./submit.zip
  2. The verification results will be shown.
      - If you see "VALIDATION SUCCEEDED", your submission appears to be
        structured correctly.
      - Otherwise, if you see "VALIDATION FAILED", you should examine the
        error message, fix the problem, and try again.
                                '''.format(conf["assignment_version"], conf["submission_name"], conf["submission_name"], conf["submission_name"]),
                                formatter_class = argparse.RawTextHelpFormatter

                    )
    parser.add_argument('path')     # First command-line argument
    args = parser.parse_args()
    # print(args.path)

    result = validate_submission(args.path)
    if not result:
        exit(1)
