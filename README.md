# Message / Stream Viewer
Assignment 3 &amp; 4 for CIS 2750: Message / Stream Viewer

**/!\ Note: Folder Version and Database Version are two seperate programs. Both programs do the same thing but store the streams differently. **

**/!\ Note: The Database Version requires that you link to your own database.**

## Folder Version
* The folder version uses a directory called "messages" to store the stream information.

## Database Version
* The Database Version uses a configurable database to store the stream information.

## Description
* A web interface for a Message / Stream Viewing program.
* Users can create streams and add authors and messages to these streams using the graphical interface.
* The graphical interface is built using PHP and a subsiquent configuration file.
* Each PHP page executes a C program that creates the HTML code for the page.

## Program Functionality
The user must be able to:
* Access the program from the web. (Uses HTML, PHP and CSS)
* Add/Remove an author
* Post messages to the stream as the current user
* View the streams they are part of.
* Change Stream
* Change Users
* View Messages Next / Previous
* Sort the message by sender name. /!\ Note: This mode does not store messages read.
* Mark All Posts as "Read"
* Start viewing messages from the last seen message on start-up


### The Config File
* The config file is saved as name.wpml
* The config file is converted by the a3.c program
* This program's output is captured by the PHP page and printed to the screen.

#### Config Flags
The configuration file uses tags to create the various HTML.
Tag Format: ._(*flags*) where _ is a letter depending on what the tag specifies.
**/!\ Note**: Any extra formatting added within the () will be added to the opening tag of the element.
* Button: Used to link to other pages either externl or internal.
    * .b() : The Tag
    * name="name" : Name of the button
  	* link="page" : Link for the button
* Horizontal Line: Creates a line that goes across the page.
    * .d() : The Tag
    * No Other Classes
* Executable: Runs the given executable.
    * .e() : The Tag
  	* exe="filename" : Name of the executable
        * Looks for the executable in the local directory, then the local bin and then the system bin.
* Headings: Creats headings using HTML headings.
    * .h() : The Tag
    * size=# : Heading Size. Where "#" is a value from 1-6
  	* text="..." :  The heading text.
    * Default : Size of 3 with text "HEADING"
* Input Form: Creates a form with text fields OR radio buttons.
    * .i()
    * action="filename" : Where the form goes on push of the select button.
    * The following three tags must be used to generate a text field.
  	    * text="..." : Text found before the field.
  	    * name="name" : Name of the text field.
  	    * value="value" : Value found inside the text field.
    * select="..." : Creates a radio group that will have a selection of that name and value.
* Link: Creates a classic hyperlink.
    * .l() : The Tag
 	  * text="..." : Hyperlink text
 	  * link="url" : URL that the link leads to.
    * Default : Text of "Link" 
* Picture: Places the image on the page.
    * .p() : The Tag
  	* image="..." : Location of the image.
  	* size=#x# : Determines the size of the image. WitdhxHeight respectively in pixels.
  	* Default : Height and Width of 100.
* Radio Form: Creates a form of radio buttons for the user to select one.
 	  * action="filename" : Where the form goes on push of the select button.
 	  * name="name" : Name of the form value / result.
 	  * value="value" : The value / name of each option.
    * Multiple Values form the different select options. The first one is the default value.
* Text: Adds the given text to the screen.
 	 * file="filename" : Name of the file. Indicates that the text is from a file.
   * text="..." : The text to be outputted. Indicates that the text is in the tag.
 	 * Default :Text of "Default text".
   * The text inside the tag or file can contain HTML.
