#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* LINKED LIST NODE TYPE: 
token: Holds the information of the node.
numWhiteSpace: # of white space that comes BEFORE the node.
numTabs: # of tabs that comes BEFORE the node.
isFunction: 1 if its a type (see is type) and is a function.
isClassVar: 1 if variable name of a class variable.
nextNode:  pointes to next node of LINKED LIST.
*/
typedef struct itemNode {
	char * token;
	int numWhiteSpace;
	int numTabs;
	int numNewLines;
	int isFunction;
	int isClassVar;
	struct itemNode * nextNode;
} item;

/*
In: File Name.
Out: File pointer to file OR exit if invalid file.
Summary: Opens the file name given and returns file pointer.
*/
FILE * openFile (char * fileName);

/*
In: Pointer to file.
Out: Parsed list of file elements.
Summary: Loops through the file and parses the information. Must take .cc file to parse correctly.
*/
item * readFile (FILE * filePointer);

/*
In: Character to check.
Out: 1 if punctuation 0 if not.
Summary: Compares char to punctuation characters EXECPT * and /
*/
int isPunctuation (char currChar);

/*
In: Character to check.
Out: 1 if white space 0 if not.
Summary: Compares character to white space characters (space,tab,newline and carrage return)
*/
int isWhiteSpace (char currChar);

/*
In: String to check if variable type.
Out: Returns 1 if variable and 0 if not
Summary: Compares string to int,float,double,struct,short,char,float,void
*/
int isVarType (char * type);

/*
In: List of the file contents.
Out: Changed list of file contents.
Summary: Loops through list and replaces the word "class" to "struct"
*/
item * replaceClass (item * programList);

/*
In: List of the file contents.
Out: Nothing.
Summary: Detects functions inside class and flags them with a 1 for later use in program.
*/
void numFunctInClass (item * programList);

/*
In: List of the file contents. 
Out: Changed lust of file contents.
Summary: Goes through every class in the code and calls other functions to make the changes. 
*/
item * changeFunctInClass (item * programList);

/*
In: The list of file contents and the name of the inFile.
Out: Nothing.
Summary: Uses the inFile (.cc) name to make another file (.c) and save the changed contents in there.
*/
void toFile (item * programList, char * inFileName);

/*
In: The file contents, list of class variables (3), the function name.
Out: Returns int for testing.
Summary: Finds the class variables used within class functions and flags them for later use.
*/
int classVarInFunct (item * programList, item * classVarList, item * paramVarList, item * localVarList, char * functionName);

/*
In: Pointer to the functionName in the list and class name string.
Out: String containing the new function name.
Summary: Creates the new function name using the class name and looping through the parameters.
*/
char * getNewFunctName(item * functionName, char * currClassName);

/*
In: Head of the function found in the file list.
Out: Returns the type of the function. (int, int **)
Summary: Starting from the head, loops until the function type is found.
*/
char * getFunctType(item * currFunctHead);

/*
In: List of the function names in the class and pointer to the current class name
Out: List with added function pointers and pointer to after list (For bracket moving).
Summary: Loops through the list of functions in te class and adds function pointers.
*/
item * insertFunctPointer(item * functionsInClass, item * currClassName, item ** afterFunctPTR);

/*
In: List of funcitons in the class, pointer to the end of the class and current class name.
Out: List with constructor function added to it.
Summary: Adds the constructor to the end of the class. Loops through every function in class.
*/
item * makeConstructor(item * functionsInClass, item * endOfClass, char * currClassName);

/*
In: String, number of white lines and spaces to add to the node.
Out: New node for the list.
Summary: Uses the information to make a new node for the list.
*/
item * makeConstructorNode(char * inStr, int numNewLines, int numWhiteSpace);

/*
In: List of file contents.
Out: Nothing.
Summary: Finds functions that are members of structs "anything.(" and looks at params to make type.
*/
void renameFunctionsInCode(item * programList);

/*
In: The file contents list and function point of view.
Out: List of all variables a function can reference.
Summary: Gathers all variables a single function can reference. Param, Local, and Global.
*/
item * listVarInFunct(item * programList, item * programList2);

/*
In: variable name, list of function vairables.
Out: Single char. (int = i)
Summary: Loops through list of function variables and returns type. Function List: int-> b -> char -> c 
*/
char getVarType(char * varName, item * functVarList);

/*
In: The struct type and the function list.
Out: String of the struct variable name. struct A myA will return myA
Summary: Gets the name of the variable calling the function inside it.
*/
char * getStructName(char * structName, item * functVarList);

/*
In: Pointer to the head of the function.
Out: Returns 1 if function and 0 if not.
Summary: Starts at the type, loops until name, if after name is a bracket then its a function, else variable.
*/
int isFunction(item * functionStart);

/*
In: The file contents list.
Out: Nothing.
Summary: Loops until a variable is found with the isClassVar as 1 then replaces it with struct pointer to it.
*/
void replaceVarInClassFunct(item * programList);

/*
In: The list containing the file contents and the struct type.
Out: Nothing.
Summary: Adds the constructor to the variables declarations. (if global adds to top of main)
*/
void addConstructor(item * programList, char * structType);

/*
In: Pointer to the file list and the function name (function name to find the funct).
Out: List of the function param types with commas and stars.
Summary: Loops through the function parameter and returns all the function param types.
*/
item * getFunctParam(item * programList, char * functionName);

/*
In: The list of file contents.
Out: List of global variable names.
Summary: Loops through the progra and finds all global variable names.
*/
item * getGlobalVarList(item * programList);

/*
In: Pointer to the class name.
Out: List of class variable names.
Summary: Loops through the class and finds all class variables.
*/
item * getClassVarList(item * programList);

/*
In: Pointer to the function name.
Out: List of param variable names for the function.
Summary: Loops through the parameters and gets a list of the variables.
*/
item * getParamVarList(item * programList);

/*
In: Pointer to the function name.
Out: List of local variable names.
Summary: Loops through the local variabls and gets a list of them.
*/
item * getLocalVarList(item * programList);


/***
      List
    Functions
***/

/*
In: Nothing.
Out: Fully initialised node.
Summary: Creates a new node and sets values to NULL or 0.
*/
item * initNode ();

/*
In: List to add to, information for new node.
Out: Item added to back of list.
Summary: Adds new node to back of list.
*/
item * addToBack (item * theList, int numWhiteSpace, int numTabs, int numNewLines, char * toBeAdded);

/*
In: The List
Out: Nothing.
Summary: Prints given list contents to screen.
*/
void printList (item * theList);

/*
In: The list.
Out: Nothing.
Summary: Loops through the list and frees the content.
*/
void freeList (item * theList);

/*
In: The List and string to check.
Out: 1 if in list and 0 if not.
Summary: Loops through the list to find the item given.
*/
int isInList (item * theList, char * token);

/*
In: Place in the list where the node should be inserted after and node to insert.
Out: List with node inserted after.
Summary: Places the node in the next spot of the list.
*/
item * insertNext(item * theList, item * newNode);
