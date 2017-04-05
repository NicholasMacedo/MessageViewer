#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 2048
#define HOSTNAME  "dursley.socs.uoguelph.ca"

#define USERNAME  "username"
#define PASSWORD  "00000000"
#define DATABASE  "database"

/* Used to print the message from the stream. */
void printMessage(char * stream){
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[MAX_QUERY];
    printf("=-=-= %s Stream Messages =-=-=\n", stream);

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "database");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("/!\\ Error: Unable to connect to database.");
        exit(0);
    }

    sprintf(query,"select message from %s",stream);  
    if(mysql_query(&mysql, query)) {
        printf("/!\\ Error: Unable to get messages from given stream.");
        mysql_close(&mysql);
        mysql_library_end();
        exit(0);
    }
    
    /* Get the results from the previous query. */
    if (!(res = mysql_store_result(&mysql))) {
        printf("/!\\ Error: Failed to retrieve result from MYSQL Query.");
        mysql_close(&mysql);
        mysql_library_end();
        exit(0);
    } else {
        while ((row = mysql_fetch_row(res))) { /* Loops through the messages in res and prints them. */
            row[0][strlen(row[0])] = '\0';
            query[0] = '\0';
            printf("%s\n", row[0]);
        }
    }
    query[0] = '\0';
    mysql_free_result(res);  
    mysql_close(&mysql);
    mysql_library_end();
    return;
}

int main(int argc, char *argv[]) {
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[MAX_QUERY];
   
    if(argc != 2) {
        printf("Two Arguments Required. \n");
    } else {
        mysql_init(&mysql);
        mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "database");
        if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
            printf("/!\\ Error: Unable to connect to database.");
            mysql_close(&mysql);
            mysql_library_end();
            exit(0);
        }
        
        if(strcmp(argv[1],"-clear") == 0) {
            /***
              Clears the information in the tables. Keeps the tables. 
            ***/
            strcpy(query, "show tables");   
            if(mysql_query(&mysql, query)) {
                printf("/!\\ Error: Unable to retrieve tables from database.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            }
            
            /* Get the results from the previous query. */
            if (!(res = mysql_store_result(&mysql))) {
                printf("/!\\ Error: Failed to retrieve result from MYSQL Query.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            } else {
                while ((row = mysql_fetch_row(res))) {
                    row[0][strlen(row[0])] = '\0';
                    query[0] = '\0';
                    sprintf(query,"truncate %s",row[0]); /* Truncate removes the info from the tables. */
                    if(mysql_query(&mysql, query)) {
                        printf("/!\\ Error: Failed to remove table data.");
                        mysql_close(&mysql);
                        mysql_library_end();
                        exit(0);
                    }
                }
            }
            mysql_free_result(res);
            query[0] = '\0';
        } else if (strcmp(argv[1],"-reset") == 0) {
            /***
              Removes All Tables from the database.
            ***/
            strcpy(query, "show tables;");   
            if(mysql_query(&mysql, query)) {
                printf("/!\\ Error: Unable to retrieve tables from database.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            }
            
            /* Get the results from the previous query. */
            if (!(res = mysql_store_result(&mysql))) {
                printf("/!\\ Error: Failed to retrieve result from MYSQL Query.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            } else {
                while ((row = mysql_fetch_row(res))) { /* Loops through the tables and removes them. */
                    row[0][strlen(row[0])] = '\0';
                    query[0] = '\0';
                    sprintf(query,"drop table %s",row[0]);
                    if(mysql_query(&mysql, query)) {
                        printf("/!\\ Error: Failed to remove table data.");
                        mysql_close(&mysql);
                        mysql_library_end();
                        exit(0);
                    }
                }
            }
            mysql_free_result(res);
            query[0] = '\0';
        } else if(strcmp(argv[1],"-posts") == 0) {
            /***
              Prints All Posts From All Streams in the database.
            ***/
            strcpy(query, "show tables;");   
            if(mysql_query(&mysql, query)) {
                printf("/!\\ Error: Unable to retrieve tables from database.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            }
            
            /* Get the results from the previous query. */
            if (!(res = mysql_store_result(&mysql))) {
                printf("/!\\ Error: Failed to retrieve result from MYSQL Query.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            } else {
                while ((row = mysql_fetch_row(res))) { /* Loops through all tables and prints the messages. Skips StreamUsers table. */
                    row[0][strlen(row[0])] = '\0';
                    query[0] = '\0';
                    if (strcmp(row[0], "StreamUsers") != 0) {
                        printMessage(row[0]);
                    }
                }
            }
            mysql_free_result(res);
            query[0] = '\0';
        } else if(strcmp(argv[1],"-users") == 0) {
            /***
              Prints All Usernames From All Streams using the StreamUsers table.
            ***/
            strcpy(query, "select distinct username from StreamUsers");   
            if(mysql_query(&mysql, query)) {
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            }
            
            /* Get the results from the previous query. */
            if (!(res = mysql_store_result(&mysql))) {
                printf("/!\\ Error: Failed to retrieve result from MYSQL Query.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            } else {
                while ((row = mysql_fetch_row(res))) { /* Prints the uernames from the query. */
                    row[0][strlen(row[0])] = '\0';
                    query[0] = '\0';
                    printf("%s\n", row[0]);
                }
            }
            mysql_free_result(res);
            query[0] = '\0';            
        } else if(strcmp(argv[1],"-streams") == 0) {
            /***
              Prints All Stream Names in the database.
            ***/
            strcpy(query, "show tables;");   
            if(mysql_query(&mysql, query)) {
                printf("/!\\ Error: Unable to retrieve tables from database.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            }
            
            /* Get the results from the previous query. */
            if (!(res = mysql_store_result(&mysql))) {
                printf("/!\\ Error: Failed to retrieve result from MYSQL Query.");
                mysql_close(&mysql);
                mysql_library_end();
                exit(0);
            } else {
                while ((row = mysql_fetch_row(res))) { /* prints all tables except StreamUsers. */
                    row[0][strlen(row[0])] = '\0';
                    query[0] = '\0';
                    if (strcmp(row[0], "StreamUsers") != 0) {
                        printf("%s\n",row[0]);
                    }
                }
            }
            mysql_free_result(res);
            query[0] = '\0';
        } else if(strcmp(argv[1],"-help") == 0) { /* Gives the User Command Info */
            printf("Possible Commands:\n");
            printf("\t-clear   | Clears all info. Keeps the streams.\n");
            printf("\t-reset   | Clears all info. Removes the streams.\n");
            printf("\t-posts   | Displays all posts in all streams.\n");
            printf("\t-users   | Displays all users in all streams.\n");
            printf("\t-streams | Displays all streams availiable.\n");
        }


        mysql_close(&mysql);
        mysql_library_end();
        
    }
    return(0);
}
