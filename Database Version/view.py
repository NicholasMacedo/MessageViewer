#!/usr/bin/python3
import sys, tty, termios, os, time, math
from datetime import datetime

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

#Gets the stream from the message string
def getStream(string):
	length = len(string)
	date = ""
	i=0;
	count = 0;
	while i<length:
		if (string[i] == 'S' and string[i+1] == 't' and string[i+2] == 'r' and string[i+3] == 'e' and string[i+4] =='a' and string[i+5] =='m' and string[i+6] ==':'):
			count = i+8;
		i = i+1
	
	while string[count] != '\n':
		date = date + string[count]
		count = count + 1
	return date

#Gets the date from the message string
def getDate(string):
	length = len(string)
	date = ""
	i=0;
	count = 0;
	while i<length:
		if (string[i] == 'D' and string[i+1] == 'a' and string[i+2] == 't' and string[i+3] == 'e' and string[i+4] ==':'):
			count = i+6;
		i = i+1
	
	while string[count] != '\n':
		date = date + string[count]
		count = count + 1
	return date

#Uses a basic bubble sort algorithm to sort the given list by date.
def sortListDate(alist):
	exchanges = True
	passnum = len(alist)-1
	while passnum > 0 and exchanges:
		exchanges = False
		for i in range(passnum):
			#strptime works the same way that strf time (used in c) works in regards to formatting a string into a date but it can compare it.
			if datetime.strptime(getDate(alist[i]), '%b. %d, %Y %I:%M %p') > datetime.strptime(getDate(alist[i+1]), '%b. %d, %Y %I:%M %p'):
				exchanges = True
				temp = alist[i]
				alist[i] = alist[i+1]
				alist[i+1] = temp
		passnum = passnum-1
	return alist

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

#Counts the number of lines it takes a message to be displayed. Used for printing.
def numMessageLines(string):
	numLines = 0
	messageLines = string.split('\n')
	for line in messageLines:
		numLines = numLines + math.ceil(len(line)/80) #Checks to see if line wraps onto next line.
	return numLines

#Uses the starting line and displays the next lines in the file.
def displayMessage(startingLine):
	displayLines = 23
	displayedLines = 0
	fileLinesUsed = 0
	linesLeft = 0
	lineNum = 0

	# Uses a temp file to store the info in for displaying easily.
	with open("messages/tempDisplay", "r") as displayFile:
		for fileLine in displayFile:
			if lineNum >= startingLine or startingLine == 0:
				linesLeft = displayLines - numMessageLines(fileLine)
				if linesLeft >= 0:
					displayLines = displayLines - numMessageLines(fileLine)
					print(fileLine,end="")
					displayedLines = displayedLines + 1
					fileLinesUsed = fileLinesUsed + 1
				else:
					return displayedLines
			lineNum = lineNum + 1
	return fileLinesUsed

#Returns the number of read posts in the given stream by the given user.
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

#Takes the given stream name and username. Takes the stream information and saves each message into a list.
def parseStreamFile(stream, username):
	messageList = []
	currMessage = ""
	numReadMessages = getNumRead(stream, username) #Start parsing messages that are unread.
	numRead = 0
	numMessages = 0
	with open("messages/"+stream+"Stream", "r") as currentStreamFile:
		with open("messages/"+stream+"StreamData", "r") as currentDataFile:
			for messageNum in currentDataFile:
				fileLocal = int(messageNum) - numRead
				currMessage = currentStreamFile.read(fileLocal)
				numRead = numRead + fileLocal
				currMessage = "Stream: " + stream + "\n" + currMessage
				numMessages = numMessages + 1
				#If the message is not read, add it to the messageList
				if (numReadMessages < numMessages) or (numReadMessages == 0):
					messageList.append(currMessage)
	return messageList

#Places the contents of the messageList into a tempDisplay file.
def putInDisplayFile(messageList):
	numLines = 0
	with open("messages/tempDisplay", "w") as displayFile:
		for message in messageList:
			displayFile.write(message)
			displayFile.write("-------------------------\n")

	with open("messages/tempDisplay", "r") as displayFile:
		for line in displayFile:
			numLines = numLines + 1
	return numLines

#Updates the stream file with the number of seen messages by the given user.
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

#Loops through the list and checks to see if the message has been viewed. If it has then update the stream.
def updateStreamView(totalNumLinesDisp,accessibleStreams,messageList,usernme):
	streamString = ""
	numLines = 0
	#Loops through the display file for the amount of lines that have been displayed.
	with open("messages/tempDisplay", "r") as displayFile:
		for line in displayFile:
			if numLines < totalNumLinesDisp:
				streamString = streamString + line
			numLines = numLines + 1

	#Checks to see if the message has been viewed. If the message has then it is marked as read.
	for message in messageList:
		if message in streamString:
			seenMessage(getStream(message),username)
		else:
			break
	return

#Main control function of the program. Handles main while loop and functionality.
def readStream(stream, accessibleStreams, username):
	userInput = ""
	numReadFileLines = 0
	tempNumRead = 0
	tempList = []
	messageList = []
	totalNumLinesDisp = 0
	lastNumLinesDisp = 0
	lastNumDLinesDisp = 0
	currentPlace = 0
	lastRun = 0

	sortToggle = 0

	if stream == "all":
		for currStream in accessibleStreams:
			if currStream != "all":
				tempList = parseStreamFile(currStream,username)
				messageList = messageList + tempList
	else:
		messageList = parseStreamFile(stream,username)
	messageList = sortListDate(messageList)
	totalFileLines = putInDisplayFile(messageList)
	lastNumDLinesDisp = displayMessage(currentPlace)

	print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
	currentPlace = currentPlace + lastNumDLinesDisp
	if currentPlace > totalNumLinesDisp and lastRun == 0 and sortToggle == 0:
		totalNumLinesDisp = currentPlace

	while userInput != "q":
		userInput = input()
		if userInput == "d":
			if currentPlace >= totalFileLines:
				currentPlace = totalFileLines
				print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
			else:
				if currentPlace < 0:
					currentPlace = 0
				lastNumDLinesDisp = displayMessage(currentPlace)
				print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
				currentPlace = currentPlace + lastNumDLinesDisp
				if currentPlace > totalNumLinesDisp and lastRun == 0 and sortToggle == 0:
					totalNumLinesDisp = currentPlace
				lastRun = 0

		elif userInput == "u":
			if currentPlace <= 23:
				print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
			else:
				if currentPlace > totalFileLines:
					currentPlace = totalFileLines
				currentPlace = currentPlace - lastNumDLinesDisp
				currentPlace = currentPlace - 23
				if currentPlace < 0:
					currentPlace = 0
				lastNumLinesDisp = displayMessage(currentPlace)
				print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
				currentPlace = currentPlace + lastNumLinesDisp
				lastRun = 1
		elif userInput == "o":
			if sortToggle == 0:
				messageList = sortListName(messageList)
				totalFileLines = putInDisplayFile(messageList)
				sortToggle = 1
			else:
				messageList = sortListDate(messageList)
				totalFileLines = putInDisplayFile(messageList)
				sortToggle = 0

			lastNumLinesDisp = 0
			lastNumDLinesDisp = 0
			currentPlace = 0
			lastRun = 0
			lastNumDLinesDisp = displayMessage(currentPlace)
			print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
			currentPlace = currentPlace + lastNumDLinesDisp

		elif userInput == "m":
			totalNumLinesDisp - totalFileLines
			if sortToggle == 1:
				messageList = sortListDate(messageList)
				totalFileLines = putInDisplayFile(messageList)
				updateStreamView(totalNumLinesDisp,accessibleStreams,messageList,username)
				messageList = sortListName(messageList)
				totalFileLines = putInDisplayFile(messageList)
			else:
				updateStreamView(totalNumLinesDisp,accessibleStreams,messageList,username)
			print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
		elif userInput == "c":
			if stream == "all":
				for currStream in accessibleStreams:
					if currStream != "all":
						tempList = parseStreamFile(currStream,username)
						messageList = messageList + tempList
			else:
				messageList = parseStreamFile(stream,username)
			messageList = sortListDate(messageList)
			totalFileLines = putInDisplayFile(messageList)
			currentPlace = 0
			lastNumDLinesDisp = displayMessage(currentPlace)
			currentPlace = currentPlace + lastNumDLinesDisp
			if currentPlace > totalNumLinesDisp and lastRun == 0 and sortToggle == 0:
				totalNumLinesDisp = currentPlace
			print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")
		elif userInput == "s":
			for i in accessibleStreams:
				print (i+" ",end="");
			print()

			stream = ""
			while stream not in accessibleStreams:
				print("Please select a stream from the list above.")
				stream = input()

			updateStreamView(totalNumLinesDisp,accessibleStreams,messageList,username)
			lastNumLinesDisp = 0
			lastNumDLinesDisp = 0
			totalNumLinesDisp = 0
			currentPlace = 0
			lastRun = 0

			sortToggle = 0

			if stream == "all":
				for currStream in accessibleStreams:
					if currStream != "all":
						tempList = parseStreamFile(currStream,username)
						messageList = messageList + tempList
			else:
				messageList = parseStreamFile(stream,username)
			messageList = sortListDate(messageList)
			totalFileLines = putInDisplayFile(messageList)
			lastNumDLinesDisp = displayMessage(currentPlace)
			currentPlace = currentPlace + lastNumDLinesDisp
			if currentPlace > totalNumLinesDisp and lastRun == 0 and sortToggle == 0:
				totalNumLinesDisp = currentPlace
			print ("u-Page Up d-Page Down o-Order Toggle m-Mark All s-Stream c-Update Stream: ",end="")


	updateStreamView(totalNumLinesDisp,accessibleStreams,messageList,username)
	os.remove("messages/tempDisplay")
	return

def userStreams(username):
	accessibleStreams = []
	for fileName in os.listdir("messages/"):
		if "StreamUsers" in fileName:
			fileNameList = fileName.split()

			with open("messages/"+fileName, "r") as currentFile:
				for fileLine in currentFile:
					if len(fileLine) > 0:
						fileLineList = fileLine.split()
						number = int(fileLineList.pop(-1))
						i = 0
						fileUserName = ""
						while i < len(fileLineList):
							fileUserName += fileLineList[i] + " "
							i += 1
						fileUserName = fileUserName.rstrip()
						if username == fileUserName:
							streamName = fileNameList[0][:len(fileNameList[0])-11]
							accessibleStreams.append(streamName)
	return(accessibleStreams)


if __name__ == "__main__":
	rowSize, columnSize = os.popen('stty size', 'r').read().split()
	sys.stdout.write("\x1b[8;{rows};{cols}t".format(rows=24, cols=80))
	username = ""
	for arg in sys.argv:
		if arg != "./view.py":
			if len(username) == 0:
				username = arg
			else:
				username = username + " "
				username = username + arg

	accessibleStreams = []
	accessibleStreams = userStreams(username)
	accessibleStreams.append("all")

	for i in accessibleStreams:
		print (i+" ",end="");
	print()

	
	stream = input()
	while stream not in accessibleStreams:
		print ("Invalid stream name. Please select a stream.")
		stream = input()

	readStream(stream, accessibleStreams, username)
	sys.stdout.write("\x1b[8;{rows};{cols}t".format(rows=rowSize, cols=columnSize))










