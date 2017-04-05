#!/usr/bin/python3
import sys, tty, termios, os, time, math, subprocess
from datetime import datetime


# THIS SCRIPT GETS THE NEWEST MESSAGE FROM THE STREEM IN USERNAME ORDER.
# This script is called using the exec from viewmain.php

if __name__ == "__main__":
	username = sys.argv[1]
	stream = sys.argv[2]
	postNum = int(sys.argv[3])

	postNum = postNum + 1
	postNum = str(postNum)

	direct_output = subprocess.check_output("./dbFunctions -newSortMessage \"" + stream + "\" " + postNum + " ", shell=True).decode("utf-8")
	if not direct_output:
		print("No More Messages In The Stream.")
	else:
		print(direct_output)
