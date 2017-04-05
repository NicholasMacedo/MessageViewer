#!/usr/bin/python3
import sys, tty, termios, os, time, math
from datetime import datetime


# THIS SCRIPT MARKS THE GIVEN NUMBER OF POSTS AS READ.
# Information given via exec command in viewmain.php



#Gets the number from the StreamUsers file.
def getNumRead(stream, username):
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
						return number
	return 0

#Increases the number of seen messages by 1.
def seenMessage(stream,username):
	fileLine = ""
	oldNumRead = 0
	newNumRead = 0
	tempString = ""
	#Creates a temp file to store the updates information
	currentStreamFileTEMP = open("messages/"+stream+"StreamUsersTEMP", "w")
	with open("messages/"+stream+"StreamUsers", "r") as streamUsersFile:
		for fileLine in streamUsersFile:

			fileLineList = fileLine.split()
			number = int(fileLineList.pop(-1))
			i = 0
			fileUserName = ""
			while i < len(fileLineList):
				fileUserName += fileLineList[i] + " "
				i += 1
			fileUserName = fileUserName.rstrip()

			if username == fileUserName:
				oldNumRead = getNumRead(stream,username)
				newNumRead = oldNumRead + 1
				tempString = fileUserName + " " + str(newNumRead) + "\n"
				currentStreamFileTEMP.write(tempString)
			else:
				currentStreamFileTEMP.write(fileLine);

	currentStreamFileTEMP.close()
	
	#Removes the old one and renames the new one to be the old one.
	os.remove("messages/"+stream+"StreamUsers")
	os.rename("messages/"+stream+"StreamUsersTEMP","messages/"+stream+"StreamUsers")
	return

if __name__ == "__main__":
	username = sys.argv[1]
	stream = sys.argv[2]
	postNum = int(sys.argv[3])
	numRead = getNumRead(stream,username)

    #Subtracts the amount of read posts from the ones read as to not mark read twice.
	addSeen = postNum - numRead

	for x in range(0,addSeen):
		seenMessage(stream,username)