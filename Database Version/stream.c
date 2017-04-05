#include "stream.h"

/* Helper Functions. 
Specific to this file. */

/* Returns 1 if stream exists. 0 if doesnt. */
int doesStreamExist(char * streamName);

/* Creates the stream files (Stream, StreamData, StreamUsers) */
void createStreamDatabase(char * streamName);

/* Update the Stream files. */
void updateStreamDatabase(char * username, char * stream, char * date, char * text);
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
    		    /* Add Message To Stream. */
    			updateStreamDatabase(st->username, token, st->date, st->text);
    		    printf("Successfully added message to %s\n", token);
    		} else {
    			printf("Error: User \"%s\" does not have appropriate permissions to post to the \"%s\" Stream.\n", st->username, token);
    		}
    	} else {
    		printf("Error: \"%s\" Stream does not exist. Files for the stream have been created. To gain permission to post to the stream please use the addauthor program.\n", token);
    		createStreamDatabase(token);
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
    	databaseAdd(token,username);
    	token = strtok (NULL, ",");
  	}
    return;
}

void databaseAdd(char * stream, char * username) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char * query = calloc(2048,sizeof(char));
	/* Estabish Database Connection */
	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, "link.to.database", "username", "password", "database", 0, NULL, 0)) {
        /*error("Could not connect to host.",&mysql);*/
        printf("Unable to connect to the database.\n");
        exit(0);
    }

    /* Create Table */
    strcat(query, "create table StreamUsers (username varchar(255), numRead int(10), stream varchar(255))");
    mysql_query(&mysql, query);
    free(query);
    query = calloc(2048,sizeof(char));
    /* If the user is not already in the table, add them. */
    strcat(query, "select count(username) from StreamUsers where username = \"");
    strcat(query, username);
    strcat(query, "\" and stream = \"");
    strcat(query, stream);
    strcat(query, "\"");
    mysql_query(&mysql, query);
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    if (strcmp(row[0],"0") == 0) {
    	free(query);
        query = calloc(2048,sizeof(char));
    	sprintf(query,"insert into StreamUsers (username,numRead,stream) values ('%s','0','%s')",username, stream);
        mysql_query(&mysql, query);
        free(query);
        query = calloc(2048,sizeof(char));
        /* Create stream Table */
        strcat(query, "create table ");
        strcat(query, stream);
        strcat(query, " (id int not null auto_increment,message text,username varchar(255), primary key(id))");
        mysql_query(&mysql, query);
    }
    free(query);
    mysql_close(&mysql);
    mysql_free_result(res);
    mysql_library_end();
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
        databaseRemove(token,username);
    	token = strtok (NULL, ",");
  	}
		return;
}

void databaseRemove(char * stream, char * username) {
	MYSQL mysql;
	char * query = calloc(2048,sizeof(char));
	/* Estabish Database Connection */
	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, "link.to.database", "username", "password", "database", 0, NULL, 0)) {
        /*error("Could not connect to host.",&mysql);*/
        printf("Unable to connect to the database.\n");
        exit(0);
    }

    /* If the user is in the table, remove them. */
    strcat(query, "delete from StreamUsers where username = \"");
    strcat(query, username);
    strcat(query, "\" and stream = \"");
    strcat(query, stream);
    strcat(query, "\"");

    /* Checks to see if the stream givn exists. If it doesnt returns. */
    mysql_query(&mysql, query);
    free(query);
    mysql_close(&mysql);
    mysql_library_end();
    return;
}

/* Returns 1 if exists and 0 if doesnt. */
int doesStreamExist(char * streamName){
	MYSQL mysql;
	int doesExist = 1;
	char * query = calloc(2048,sizeof(char));
	/* Estabish Database Connection */
	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, "dursley.socs.uoguelph.ca", "nmacedov", "0889469", "nmacedov", 0, NULL, 0)) {
        /*error("Could not connect to host.",&mysql);*/
        printf("Unable to connect to the database.\n");
        exit(0);
    }

    strcat(query, "select * from ");
    strcat(query, streamName);

    /* Checks to see if the stream givn exists. If it doesnt returns. */
    if (mysql_query(&mysql, query)){
        doesExist = 0;
    }
    free(query);
    mysql_close(&mysql);
    mysql_library_end();
    return (doesExist);
}

/* Creates New Stream Tables in the Database */
void createStreamDatabase(char * streamName){
	MYSQL mysql;
	char * query = calloc(2048,sizeof(char));
	/* Estabish Database Connection */
	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, "dursley.socs.uoguelph.ca", "nmacedov", "0889469", "nmacedov", 0, NULL, 0)) {
        /*error("Could not connect to host.",&mysql);*/
        printf("Unable to connect to the database.\n");
        exit(0);
    }

    /* Create stream Table */
    strcat(query, "create table ");
    strcat(query, streamName);
    strcat(query, " (id int not null auto_increment,message text,username varchar(255), primary key(id))");
    mysql_query(&mysql, query);

    free(query);
    mysql_close(&mysql);
    mysql_library_end();
    return;
}

/* Updates the Stream file using the information given. */
void updateStreamDatabase(char * username, char * stream, char * date, char * text){
	MYSQL mysql;
	char * query = calloc(2048,sizeof(char));
	char * message = calloc(4096,sizeof(char));

	/* Estabish Database Connection */
	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, "dursley.socs.uoguelph.ca", "nmacedov", "0889469", "nmacedov", 0, NULL, 0)) {
        /*error("Could not connect to host.",&mysql);*/
        printf("Unable to connect to the database.\n");
        exit(0);
    }

    sprintf(message, "Sender: %s\nDate: %s\n%s", username, date, text);

    sprintf(query,"insert into %s (message,username) values ('%s','%s')",stream, message,username);
	mysql_query(&mysql, query);
    free(query);
    free(message);
    mysql_close(&mysql);
    mysql_library_end();
    return;
}


/* Checks to see if the given user has permissions to the given stream. */
int hasPermissions(char * stream, char * username) {
	MYSQL mysql;
	int hasPerms = 0;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char * query = calloc(2048,sizeof(char));
	/* Estabish Database Connection */
	mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, "dursley.socs.uoguelph.ca", "nmacedov", "0889469", "nmacedov", 0, NULL, 0)) {
        /*error("Could not connect to host.",&mysql);*/
        printf("Unable to connect to the database.\n");
        exit(0);
    }

    /* If the user is not already in the table, add them. */
    strcat(query, "select count(username) from StreamUsers where username = \"");
    strcat(query, username);
    strcat(query, "\" and stream = \"");
    strcat(query, stream);
    strcat(query, "\"");
    mysql_query(&mysql, query);
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    if (strcmp(row[0],"1") == 0) {
    	hasPerms = 1;
    }
    free(query);
    mysql_close(&mysql);
    mysql_free_result(res);
    mysql_library_end();
    return (hasPerms);
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









