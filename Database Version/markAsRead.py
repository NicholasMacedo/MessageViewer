#!/usr/bin/python3
import sys, tty, termios, os, time, math, subprocess
from datetime import datetime


# THIS SCRIPT MARKS THE GIVEN NUMBER OF POSTS AS READ.
# Information given via exec command in viewmain.php

if __name__ == "__main__":
	username = sys.argv[1]
	stream = sys.argv[2]
	postNum = int(sys.argv[3])
	direct_output = subprocess.check_output("./dbFunctions -getRead \"" + username + "\" " + stream + " ", shell=True).decode("utf-8") 
	numRead = int(direct_output)
    #Subtracts the amount of read posts from the ones read as to not mark read twice.
	addSeen = postNum - numRead

	numRead = numRead + addSeen

	numRead = str(postNum)
	direct_output = subprocess.check_output("./dbFunctions -updateRead \"" + username + "\" \"" + stream + "\" " + " " + numRead + " ", shell=True).decode("utf-8")
