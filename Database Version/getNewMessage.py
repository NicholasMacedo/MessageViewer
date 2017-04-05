#!/usr/bin/python3
import sys, tty, termios, os, time, math, subprocess
from datetime import datetime


# THIS SCRIPT GETS THE NEWEST MESSAGE FROM THE STREEM IN CRONOLOGICAL ORDER.
# This script is called using the exec from viewmain.php

if __name__ == "__main__":
	username = sys.argv[1]
	stream = sys.argv[2]
	postNum = int(sys.argv[3])
	direct_output = subprocess.check_output("./dbFunctions -getRead \"" + username + "\" " + stream + " ", shell=True).decode("utf-8") 
	numReadMessages = int(direct_output)
	direct_output = subprocess.check_output("./dbFunctions -getNumMessages \"" + stream + "\" ", shell=True).decode("utf-8") 
	totalMessages = int(direct_output)

	postNum = postNum + 1
	postNum = str(postNum)

	direct_output = subprocess.check_output("./dbFunctions -newMessage \"" + stream + "\" " + postNum + " ", shell=True).decode("utf-8")
	if not direct_output:
		print("No More Messages In The Stream.")
	else:
		print(direct_output)

	postNum = int(postNum)
	if postNum > numReadMessages and postNum <= totalMessages: #Only adds the message as read if the postNum is greater then the number of read messages marked.
		postNum = str(postNum)
		direct_output = subprocess.check_output("./dbFunctions -updateRead \"" + username + "\" \"" + stream + "\" " + " " + postNum + " ", shell=True).decode("utf-8")

