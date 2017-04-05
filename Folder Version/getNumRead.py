#! /usr/bin/python3

# THIS SCRIPT GETS THE AMOUNT OF POSTS READ BY THE GIVEN USER IN THE GIVEN STREAM
# Called from viewmain.php


import sys,os
username = sys.argv[1]
stream = sys.argv[2]
with open("messages/"+stream+"StreamUsers", "r") as currentStreamFile:
	for fileLine in currentStreamFile:
				fileLineList = fileLine.split()
				number = int(fileLineList.pop(-1))
				i = 0
				fileUserName = ""
				while i < len(fileLineList):
					fileUserName += fileLineList[i] + " "
					i += 1
				fileUserName = fileUserName.rstrip()
				if username == fileUserName:
					print (number,end="")