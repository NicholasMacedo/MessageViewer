#!/usr/bin/python3
import sys, tty, termios, os, time, math
from datetime import datetime


# THIS SCRIPT GETS THE NEWEST MESSAGE FROM THE STREEM IN CRONOLOGIVAL ORDER.
# This script is called using the exec from viewmain.php


#Gets the read number from StreamUsers
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

#Marks the message as seen to the StreamUsers file.
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
	unseen = 0

	currMessage = ""
	numReadMessages = getNumRead(stream, username) #Start parsing messages that are unread.
	if len(sys.argv) > 4:
		postNum = postNum - 2

	numRead = 0
	numMessages = 0
	with open("messages/"+stream+"Stream", "r") as currentStreamFile:
		with open("messages/"+stream+"StreamData", "r") as currentDataFile:
			for messageNum in currentDataFile:
				fileLocal = int(messageNum) - numRead
				currMessage = currentStreamFile.read(fileLocal)
				numRead = numRead + fileLocal
				numMessages = numMessages + 1
				#If the message is not read, add it to the messageList
				if (postNum < numMessages) or (postNum == 0):
					print(currMessage);
					if postNum >= numReadMessages: #Only adds the message as read if the postNum is greater then the number of read messages marked.
						seenMessage(stream,username)
					sys.exit();
	print("End of Stream. No More Messages to Display!") #Only prints if no message is displayed.
