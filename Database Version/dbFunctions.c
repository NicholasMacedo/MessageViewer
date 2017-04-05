#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 2048
#define HOSTNAME  "dursley.socs.uoguelph.ca"

#define USERNAME  "nmacedov"
#define PASSWORD  "00000000"
#define DATABASE  "nmacedov"

int main(int argc, char *argv[]) {
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[MAX_QUERY];
    int firstLoop = 0;
   
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "database");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("/!\\ Error: Unable to connect to database.");
        exit(0);
    }
    
    if(strcmp(argv[1],"-newMessage") == 0) { /* argv[2] = username && argv[3] = postNum */
        /***
          Gets the message from the stream using the given number.
        ***/
        sprintf(query, "select message from %s where id='%s'",argv[2],argv[3]);   
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
                printf("%s", row[0]); /* Prints the message */
            }
        }
        mysql_free_result(res);
        query[0] = '\0';
    } else if(strcmp(argv[1],"-newSortMessage") == 0) {  /* argv[2] = username && argv[3] = postNum */
        /***
          Gets the message from the stream using the given number. Sorts the stream by the username column.
        ***/
        sprintf(query, "select message from %s order by username",argv[2]);   
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
            int test = atoi(argv[3]); /* Gets the post number from command line and subtracts by one.*/
            test --;
            while ((row = mysql_fetch_row(res))) {
                if (firstLoop == test) {
                    row[0][strlen(row[0])] = '\0';
                    printf("%s", row[0]); /* Finds the post number and prints it. */
                    break;
                }
                firstLoop ++;
            }
        }
        mysql_free_result(res);
        query[0] = '\0';
    } else if(strcmp(argv[1],"-updateRead") == 0) {  /* argv[2] = username && argv[3] = stream && argv[4] = numRead */
        /***
          Updates the numRead column in the StreamUsers table for the given username in stream.
        ***/
        sprintf(query, "update StreamUsers SET numRead='%s' WHERE username='%s' and stream='%s'", argv[4], argv[2], argv[3]);   
        if(mysql_query(&mysql, query)) {
            printf("/!\\ Error: Unable to retrieve tables from database.");
            mysql_close(&mysql);
            mysql_library_end();
            exit(0);
        }
        query[0] = '\0';
    } else if(strcmp(argv[1],"-getRead") == 0) {  /* argv[2] = username && argv[3] = stream */
        /***
          Gets the numRead of the username given from the StreamUsers table using the stream.
        ***/
        sprintf(query, "select numRead from StreamUsers where username='%s' and stream='%s'", argv[2], argv[3]);  
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
                printf("%s", row[0]); /* Prints numRead */
            }
        }
        mysql_free_result(res);
        query[0] = '\0';
    } else if(strcmp(argv[1],"-getNumMessages") == 0) {  /* argv[2] = stream */
        /***
          Counts the number of messages in the given stream.
        ***/
        sprintf(query, "select count(message) from %s", argv[2]);  
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
                printf("%s", row[0]); /* Prints the number. */
            }
        }
        mysql_free_result(res);
        query[0] = '\0';
    } else if(strcmp(argv[1],"-getStreams") == 0) {  /* argv[2] = username */
        /***
          Gets the streams the given user is part of. Creates the form.
        ***/
        sprintf(query, "select stream from StreamUsers where username='%s'", argv[2]);   
        if(mysql_query(&mysql, query)) {
            printf("<p>The user %s has no streams to view.</p><br> <p>Please use the Add / Remove Author page to give the user access to streams.</p>\n",argv[2]);
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
            if (mysql_num_rows(res) == 0) { /* If returns 0 then user is not part of a stream. */
                    printf("<p>The user %s has no streams to view.</p><br> <p>Please use the Add / Remove Author page to give the user access to streams.</p>\n",argv[2]);
                    mysql_free_result(res);
                    query[0] = '\0';
                    mysql_close(&mysql);
                    mysql_library_end();
                    exit(0);
            } else { /* Prints the form with the streams they are part of. */
                printf("<form action=\"viewmain.php\" method=\"POST\">\n");
                while ((row = mysql_fetch_row(res))) {
                    row[0][strlen(row[0])] = '\0';
                    if (firstLoop == 0) {
                        printf("    <input type=\"radio\" name=\"radio\" value=\"%s\" checked>%s<br>\n", row[0] ,row[0]);
                        firstLoop = 1;
                    } else {
                        printf("    <input type=\"radio\" name=\"radio\" value=\"%s\">%s<br>\n", row[0] ,row[0]);
                    }
                }
                printf("    <input type=\"hidden\" name=\"username\" value=\"%s\">\n",argv[2]);
                printf("    <input type=\"hidden\" name=\"postNum\" value=\"-1\">\n");
                printf("    <input class=\"submitButton\" type=\"submit\" value=\"Submit\">\n");
                printf("</form>\n");
            }
        }
        mysql_free_result(res);
        query[0] = '\0';
    }

    mysql_close(&mysql);
    mysql_library_end();
        
    return(0);
}
