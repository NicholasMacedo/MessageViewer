#! /usr/bin/python3

# THIS SCRIPT GETS THE AMOUNT OF POSTS READ BY THE GIVEN USER IN THE GIVEN STREAM
# Called from viewmain.php


import sys,os, subprocess
username = sys.argv[1]
stream = sys.argv[2]

direct_output = subprocess.check_output("./dbFunctions -getRead \"" + username + "\" \"" + stream + "\" ", shell=True).decode("utf-8")
print(direct_output)