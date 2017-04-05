#include "stream.h"

/* Helper Functions. 
Specific to this file. */

/* Returns 1 if stream exists. 0 if doesnt. */
int doesStreamExist(char * streamName);

/* Creates the stream files (Stream, StreamData, StreamUsers) */
void createFiles(char * streamName);

/* Update the Stream files. */
void updateStreamFile(char * username, char * stream, char * date, char * text);
void updateStreamDataFile(char * stream);
void updateStreamUsersFile(char * stream, char * username);

/* Returns 1 if user has permissions. 0 if doesnt. */
int hasPermissions(char * stream, char * username);

/* Gets the username from the given string. */
char * getUserName(char * streamUsersFileLine);

/*
 - Adds new posting to the given stream (st->streamname)
 - Stream not exist: make a new one
 - Stream Exists, add it to the stream.
 */
void updateStream(struct userPost * st) {
	char * token = strtok (st->streamname,",");
	while (token != NULL) {
    	/* If the stream exists, and the user has permissions: add user. */
    	if (doesStreamExist(token) == 1) {
    		if (hasPermissions(token, st->username) == 1) {
    		    updateStreamFile(st->username,token,st->date,st->text);
    		    updateStreamDataFile(token);
    		    printf("Successfully added message to %s\n", token);
    		} else {
    			printf("Error: User \"%s\" does not have appropriate permissions to post to the \"%s\" Stream.\n", st->username, token);
    		}
    	} else {
    		printf("Error: \"%s\" Stream does not exist. Files for the stream have been created. To gain permission to post to the stream please use the addauthor program.\n", token);
    		createFiles(token); /* If the stream doesnt exist. Give error and create files. */
    	}
    	token = strtok (NULL, ",");
  	}
    return;
}

/*
 - Adds user (username) to the streams (list).
 - list is a comma seperated list of stream names. (Tokenize by comma)
 - Stream Not Exist: make a new stream with that name.
*/
void addUser(char * username, char * list){
	char * token = strtok (list,",");
	/* Loops through the tokens in the list. */
    while (token != NULL) {
    	/* If the stream exists then it just adds the user. Else, creates stream and then adds. */
    	if (doesStreamExist(token) == 1) {
    		updateStreamUsersFile(token, username);
    	} else {
    		createFiles(token);
    		updateStreamUsersFile(token, username);
    	}
    	token = strtok (NULL, ",");
  	}
    return;
}

/*
 - Remove user (username) from the streams (list).
 - list is a comma seperated list of stream names. (Tokenize by comma)
 - Stream Not Exist: Skip it as there is no user to remove from that "stream"
*/
void removeUser(char * username, char * list){

	char * token = strtok (list,",");
	/* Loops through the usernames given in the list */
	while (token != NULL) {
    	/* Removes from stream if the stream exists. */
	    if (doesStreamExist(token) == 1) {
	    	char * originalFile = calloc(2048,sizeof(char));
	    	/* Opens the stream user file. */
			FILE * originalFP;
			strcpy(originalFile,"messages/");
			strcat(originalFile,token);
			strcat(originalFile,"StreamUsers");
			originalFP = fopen(originalFile,"r");

			/* Creates a new file to copy the info into. */
			char * newFile = calloc(2048,sizeof(char));
			FILE * newFP;
			strcpy(newFile,originalFile);
			strcat(newFile,"temp");
			newFP = fopen(newFile,"w");
			char * string = calloc(2048,sizeof(char));
			int number = 0;

            /* Loops through the StreamUsers file. 
               If the string from the file contains the username
               then it does not print it to the new file.*/
			while (fgets(string,2048,originalFP) != NULL) {
			    string[strlen(string)-1] = '\0';
				char * tstring = getUserName(string);
		        if(strcmp(username,tstring) != 0) {
		             fprintf(newFP, "%s %d\n", tstring, number);
		        }
		        free(string);
		        free(tstring);
		        string = calloc(2048,sizeof(char));
		    }
		    /* Closes the files, removes the old file and renames the new one. */
		    fclose(originalFP);
		    fclose(newFP);
		    remove(originalFile);
		    rename(newFile, originalFile);
			free(originalFile);
			free(newFile);
			free(string);
		}
    	token = strtok (NULL, ",");
  	}
		return;
}

/* Returns 1 if exists and 0 if doesnt. */
int doesStreamExist(char * streamName){
	char * fileName = calloc(2048,sizeof(char));
	FILE * doesExist;
	/* Uses the StreamData file to check if the stream exists.
	   Assumption: If one file exists, the entire stream exists.*/
	strcpy(fileName,"messages/");
	strcat(fileName,streamName);
	strcat(fileName,"StreamData");
	doesExist = fopen(fileName,"r");
	if (doesExist == NULL) {
		free(fileName);
		return(0);
	}
	fclose(doesExist);
	free(fileName);
	return(1);
}

/* Creates New Stream Files */
void createFiles(char * streamName){
	char * fileName = calloc(2048,sizeof(char));
	FILE * newFile;
	strcpy(fileName,"messages/");
	strcat(fileName,streamName);
	strcat(fileName,"Stream");
	newFile = fopen(fileName,"w");
	fclose(newFile);
	free(fileName);

	fileName = calloc(2048,sizeof(char));
	strcpy(fileName,"messages/");
	strcat(fileName,streamName);
	strcat(fileName,"StreamData");
	newFile = fopen(fileName,"w");
	fclose(newFile);
	free(fileName);

	fileName = calloc(2048,sizeof(char));
	strcpy(fileName,"messages/");
	strcat(fileName,streamName);
	strcat(fileName,"StreamUsers");
	newFile = fopen(fileName,"w");
	fclose(newFile);
	free(fileName);
}

/* Updates the Stream file using the information given. */
void updateStreamFile(char * username, char * stream, char * date, char * text){
	char * fileName = calloc(2048,sizeof(char));
	FILE * streamFile;
	/* Opens the Stream file with append for easy file input.*/
	strcpy(fileName,"messages/");
	strcat(fileName,stream);
	strcat(fileName,"Stream");
	streamFile = fopen(fileName,"a");

    /* Prints the information to the file. */
	fprintf(streamFile, "Sender: %s\n", username);
	fprintf(streamFile, "Date: %s\n", date);
	fprintf(streamFile, "%s", text);

	free(fileName);
	fclose(streamFile);
}

/* Update the StreamData file. */
void updateStreamDataFile(char * stream) {
	int file=0;
	char * fileName = calloc(2048,sizeof(char));
	strcpy(fileName,"messages/");
	strcat(fileName,stream);
	strcat(fileName,"Stream");
	/* Opens the Stream file for reading by the fstat function. */
	file = open(fileName,O_RDONLY);
	struct stat fileStat;
	if(fstat(file,&fileStat) < 0) { /* Returns if the file fails to open. */
		return;
	}
	free(fileName);
	fileName = calloc(2048,sizeof(char));

	/* Opens the StreamData file and puts the size of the file given by fstat in it. */
	FILE * streamDataFile;
	strcpy(fileName,"messages/");
	strcat(fileName,stream);
	strcat(fileName,"StreamData");
	streamDataFile = fopen(fileName,"a");
	fprintf(streamDataFile, "%d\n", (int)fileStat.st_size);
	free(fileName);
	fclose(streamDataFile);
}

/* Updates the StreamUsers file with the username */
void updateStreamUsersFile(char * stream, char * username) {
	char * fileName = calloc(2048,sizeof(char));
	/* Setup file and open with append. */
	FILE * streamDataFile;
	strcpy(fileName,"messages/");
	strcat(fileName,stream);
	strcat(fileName,"StreamUsers");
	streamDataFile = fopen(fileName,"a");
	/* Put the username in the file and set the number to 0. */
	fprintf(streamDataFile, "%s ", username);
	fprintf(streamDataFile, "0\n");
	free(fileName);
	fclose(streamDataFile);
}

/* Checks to see if the given user has permissions to the given stream. */
int hasPermissions(char * stream, char * username) {
    char * streamFile = calloc(2048,sizeof(char));
	FILE * streamFP;
	/* Setup the fileName */
	strcpy(streamFile,"messages/");
	strcat(streamFile,stream);
	strcat(streamFile,"StreamUsers");
	streamFP = fopen(streamFile,"r");
	char * fileLine = calloc(2048,sizeof(char));
	int permissions = 0;

    /* Loop through the StreamUsers file and compare the usernames to the given username. */
    while (fgets(fileLine,2048,streamFP) != NULL) {
	    fileLine[strlen(fileLine)-1] = '\0';
		char * currName = getUserName(fileLine);
        if(strcmp(username,currName) == 0) {
             permissions = 1;
        }
        free(fileLine);
        free(currName);
        fileLine = calloc(2048,sizeof(char));
	 }

	fclose(streamFP);
	free(streamFile);
	free(fileLine);
	return(permissions);
}

/* Returns the username from the given string. EG: "test 0" will return "test" */
char * getUserName(char * streamUsersFileLine) {
    char * username = calloc(2048,sizeof(char));
    int numSpaces = 0;
    int numTokens = 0;
    int i = 0;

    /* Count the number of characters to determine the token. EG: 3 spaces = 2 username tokens. */
    for (i = 0; i < strlen(streamUsersFileLine); ++i) {
    	if (streamUsersFileLine[i] == ' ') {
    		numSpaces ++;
    	}
    }

    /* Loop through each character until the amount of spaces matches. */
    for (i = 0; i < strlen(streamUsersFileLine); ++i) {
    	if (streamUsersFileLine[i] == ' ' && (numSpaces != numTokens)) {
    		username[i] = streamUsersFileLine[i];
    		numTokens ++;
    	} else if (numSpaces != numTokens) {
    		username[i] = streamUsersFileLine[i];
    	}
    }
	username[strlen(username)-1] = '\0';
    return (username);
}









