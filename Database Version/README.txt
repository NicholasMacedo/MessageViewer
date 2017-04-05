Program Assumptions:

The following list contains my understanding/ what I did for some gray areas of the code.

	- Connection to the database is enabled and there are no issues connecting to the database.
	- Username entered by the user is NOT userchecked. Do not enter " " or "'" or """ in username.
	- Special characters when posting to the stream may cause it to not post.
	- Stream names will NOT have a space OR special characters in it
	- LIMIT of 2040 characters when entering post.
	- Repeated spamming of refresh could cause errors when using the GUI.
	- ASSUMED MAKE IS TYPED BEFORE GOING TO INDEX.PHP
	- When INDEX OR ADDAUTHOR is selected, the USERNAME and STREAM is lost. This is a design choice.
	  If they want to view again they must go through the viewing process of entering username & stream selection.
	- Connection to the internet is availiable. (USES BOOTSTRAP FOR THE CSS STYLE HELP)
	- User will not touch or use the inspect element to play around with code. The functionality is IFFY and may break the output.
	- When viewign the streams, if the info is removed from the database then the program will continue as if it still exists. Functionality is unknown if the files existed and then were deleted mid viewing secession.


dbFunctions:
	- dbFunctions is an INTERNALLY used program. It was created for use by the system only.
	- Assumptions of correct usage and commandline arguments are made due to being automated by
	the PHP files.


The C Program:
	- Added .c flag with a pos=1 for open div and pos=0 for close div. Garbage is added to the opening div tag.
	- The text from a file goes straight to the screen. any extra information is added BEFORE the file is printed straight to the screen.
	- The extra garbage in the .e flag is IGNORED due to the execution happening on the server.
	- The output of executions is routed to /dev/null as per the discussion with CIS 2750 TA John.
	- Repeated tags take the lates value. text="a",text="b" will sesult in text b being chosen.
	- The links are given with https:// if from an ourside source. Therefore google is https://google.com

Limitations:
	- Requires Python 3 to run.
	- Use in order of availiable buttons on the page and NOT jumping out of order and to various pages.
	- User uses the system properly. Meaning not jumping to random pages and playing with the URL.
	-Subjected to SQL Injections. 
	- Server requires connection to the MYSQL server and the server is functioning. the GUI does not check to see if the MYSQL server information is correct. The error will be printed to the location the information will normally go.
	- "ALL" stream option does not exist.
	- Username/Stream will NOT be JUST a space and will not contain a space or special characters.
