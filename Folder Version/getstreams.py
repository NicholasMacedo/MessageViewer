#! /usr/bin/python3
import sys,os


# THIS SCRIPT IS USED TO GET THE STREAMS AND RETURNS A FORM IN HTML.
# This is used by getstreams.php to display the streams.


username = sys.argv[1]
first = 0
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

#If the user has no streams then print message and do not display a form.
if len(accessibleStreams) == 0:
	print("<p>The user "+ username +" has no streams to view.</p><br> <p>Please use the Add / Remove Author page to give the user access to streams.</p>");
	sys.exit();

#Print the form and make the first one selected auto.
print ("<form action=\"viewmain.php\" method=\"POST\">")
for stream in accessibleStreams:
	if first == 0:
		print ("    <input type=\"radio\" name=\"radio\" value=\""+ stream +"\" checked>"+ stream +"<br>")
		first = 1
	else:
		print ("    <input type=\"radio\" name=\"radio\" value=\""+ stream +"\">"+ stream +"<br>")
print ("    <input type=\"hidden\" name=\"username\" value=\""+ username +"\">")
print ("    <input type=\"hidden\" name=\"postNum\" value=\"-1\">")
print ("    <input class=\"submitButton\" type=\"submit\" value=\"Submit\">");
print ("</form>")