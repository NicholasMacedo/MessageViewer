Program Assumptions:

The following list contains my understanding/ what I did for some gray areas of the code.

	- If one Stream file exists, the other two do.
	- Python program requires a messages directory to be present. Meaning that Post or addauthor must be run before using view.py.
	- AddUser does not check to see if the given user exists in the stream. Therefore, will create two versions of this user.
	- Username entered by the user is NOT userchecked.
	- Special characters when posting to the stream cause it to not post.
	- Stream names will NOT have a space OR special characters in it
	- LIMIT of 2040 characters when entering info
	- Added chmod to the Makefile to fix permissions error. Might give error when typing "make" if make folder exists OR unable to use chmod.
	- Repeated spamming of refresh could cause errors when using the GUI.
	- ASSUMED MAKE IS TYPED BEFORE GOING TO INDEX.PHP
	- When INDEX OR ADDAUTHOR is selected, the USERNAME and STREAM is lost. This is a design choice. If they want to view again they must go through the viewing process.
	- Connection to the internet is availiable. (USES BOOTSTRAP FOR THE CSS STYLE HELP)
	- User will not touch the inspect element and play around with code. The functionality is IFFY and may break the output.


The C Program:
	- Added .c flag with a pos=1 for open div and pos=0 for close div. Garbage is added to the opening div tag.
	- The text from a file goes straight to the screen. any extran information is added BEFORE the file is printed straight to the screen.
	- The extra garbage in the .e flag is IGNORED due to the execution happening on the server.
	- The output of executions is routed to /dev/null as per the discussion with CIS 2750 TA John.
	- Repeated tags take the lates value. text="a",text="b" will sesult in text b being chosen.
	- The links are given with https:// if from an ourside source. Therefore google is https://google.com

Limitations:
- Requires Python 3 to run.
- Use in order of availiable buttons on the page and NOT jumping out of order and to various pages.
- User uses the system properly. Meaning not jumping to random pages and playing with the URL.
- Server requires permissions to the files. Assumed permissions work.
- "ALL" stream option does not exist.
- Username/Stream will NOT be JUST a space.
- Usernames can have a space in it but NO special characters. (tested and works but iffy)
