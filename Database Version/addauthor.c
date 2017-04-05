#include "stream.h"

int main(int argc, char const *argv[]) {
	int remove = 0;
    int i = 0;
	char * userName = calloc(2048,sizeof(char));
    char * streams = calloc(2048,sizeof(char));

    /* Only one username at a time. Gathers username from args. */

    if (argc < 2) {
        printf("Incorrect number of args.\n");
        exit(0);
    }


    /* The -A is for the use of AUTOMATION inside the PHP script. */
    if (strcmp(argv[1],"-A") == 0) {
        strcat(userName,argv[2]);
        strcat(streams,argv[3]);
        if (argc > 4) {
                remove = 1;
        }
    } else {
    	for (i = 0; i < argc; ++i) {
    		if (strcmp(argv[i],"-r") == 0) {
    			remove = 1;
    		} else {
                if (strcmp(argv[i],"./addauthor") != 0) {
                    if (strlen(userName) == 0) {
                       strcat(userName,argv[i]);
                    } else {
                        strcat(userName," ");
                        strcat(userName,argv[i]);
                    }
                }
    		}
    	}
    }
    
    /* Gets the comma seperated streams from the user. */
    if (strcmp(argv[1],"-A") != 0) {
        printf("list streams: ");
        fgets(streams,2048,stdin);
        streams[strlen(streams)-1] = '\0';
    }
    /* If the -r flag is present, removve. Else add user to the given streams. */
    if (remove == 1) {
        removeUser(userName, streams);
    } else {
        addUser(userName, streams);
    }

    free(userName); 
    free(streams);
	return (0);
}