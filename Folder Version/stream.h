#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/* User Post  structure. */
struct userPost {
    char * username;
    char * streamname;
    char * date;
    char * text;
};

/* Functions for stream manipulation from assignent */
void updateStream(struct userPost * st);
void addUser(char * username, char * list);
void removeUser(char * username, char * list);