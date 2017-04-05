#!/usr/bin/python3
import sys, tty, termios, os, time, math
from datetime import datetime


# THIS SCRIPT IS USED TO GET THE NEWEST MESSAGE FROM THE STREAM SORTED IN NAME ORDER.
# Called form viewmain.php using the exec.


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

#Gets the sender name from the message string
def getName(string):
	length = len(string)
	date = ""
	i=0;
	count = 0;
	while i<length:
		if (string[i] == 'S' and string[i+1] == 'e' and string[i+2] == 'n' and string[i+3] == 'd' and string[i+4] =='e' and string[i+5] =='r' and string[i+6] ==':'):
			count = i+8;
		i = i+1
	
	while string[count] != '\n':
		date = date + string[count]
		count = count + 1
	return date

#Uses a basic bubble sort algorithm to sort the given list by name
def sortListName(alist):
	exchanges = True
	passnum = len(alist)-1
	while passnum > 0 and exchanges:
		exchanges = False
		for i in range(passnum):
			if getName(alist[i]) > getName(alist[i+1]):
				exchanges = True
				temp = alist[i]
				alist[i] = alist[i+1]
				alist[i+1] = temp
		passnum = passnum-1
	return alist



#Takes the given stream name and username. Takes the stream information and saves each message into a list.
def parseStreamFile(stream, username):
	messageList = []
	currMessage = ""
	numReadMessages = 0
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
				if (numReadMessages < numMessages) or (numReadMessages == 0):
					messageList.append(currMessage)
	return messageList

if __name__ == "__main__":
	username = sys.argv[1]
	stream = sys.argv[2]
	sortNum = int(sys.argv[3])
	unseen = 0

	currMessage = ""
	numReadMessages = getNumRead(stream, username) #Start parsing messages that are unread.
	messageList = []
	messageList = parseStreamFile(stream, username) #Sorts messages.
	sortListName(messageList)
	messageList.append("End of Stream. No More Messages to Display!")
	print(messageList[sortNum])





