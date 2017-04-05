#include "stream.h"

class PostEntry {

    struct userPost post;

    /* Reads in the user input (stream name & text) and places it into the struct. */
    void readInput(){
        char * input = calloc(2048,sizeof(char));
        int firstLine = 0;

        while(1) {
            if (firstLine == 0) {
                /* Gets the stream from the user and stores it in the struct. */
                printf("stream: ");
                fgets(post.streamname,2048,stdin);
                post.streamname[strlen(post.streamname)-1] = '\0';
                printf("enter text: ");
                firstLine = 1;
            } else {
                printf("- ");
            }
            fflush(NULL);
            /* Loops until ctrl+d which is EOF/NULL */
            if (fgets(input,2048,stdin) == NULL) {
                printf("ctrl-d\n");
                break;
            }
            strcat(post.text,input);
        }
        free(input);
    }

    /* Creates the variables within the struct. */
    void formatEntry(){ 
        post.username = calloc(2048,sizeof(char));
        post.streamname = calloc(2048,sizeof(char));
        post.date = calloc(2048,sizeof(char));
        post.text = calloc(2048,sizeof(char));
    }

    /* Uses the time library function to retrieve, format and save the date. */
    void getTimeDate(){
        time_t unformatedTime;
        struct tm *timeStruct;
        time(&unformatedTime); /* Gets the raw time from the system. */
        timeStruct = localtime(&unformatedTime); /* Formats that time inside of the timeStruct function. */
        strftime(post.date,2048,"%b. %d, %Y %I:%M %p", timeStruct); /* Formats the date string using the time struct and instructions from the strftime function. */
    }

    /* Uses the struct to update the stream if needed. */
    void submitPost(){
        updateStream(&post);
    }

    /* Frees the memory used by the struct. */
    void freePostVariables(){
        free(post.username);
        free(post.streamname);
        free(post.date);
        free(post.text);
    }

    /* Sets the value of the username. */
    void setUserName(char * inUserName){
        strcpy(post.username,inUserName);
        strcat(post.username,"\0");
    }
};


int main(int argc, char const *argv[]) {
    class PostEntry newPost;
    int i;
    char * userName = calloc(2048,sizeof(char));


    /* Uses the -A flag to indicate automation by the PHP program. */
    if (strcmp(argv[1],"-A") == 0) {
        strcpy(userName,argv[2]);
        newPost.formatEntry();
        newPost.setUserName(userName);
        strcpy(newPost.post.streamname,argv[3]);

        for (i = 0; i < strlen(argv[4]); ++i) {
            char test = argv[4][i];
            strncat(newPost.post.text,&test,1);
        }
        strcat(newPost.post.text,"\n");
        newPost.getTimeDate();
        newPost.submitPost();
        newPost.freePostVariables();
        free(userName);
        return(0);
    }

    /* Gets the username from the commandline. Only one username can be submitted. */
    for (i = 0; i < argc; ++i) {
        if (strcmp(argv[i],"./post") != 0) {
            if (strlen(userName) == 0) {
               strcat(userName,argv[i]);
            } else {
                strcat(userName," ");
                strcat(userName,argv[i]);
            }
        }
    }

    /* Checks to see if the directory messages exists. If it doesnt, creates it. */
    struct stat doesMessagesExist = {0};
    if (stat("messages", &doesMessagesExist) == -1) {
        mkdir("messages", 0700);
    }

    /* Calls the functions inside the class to make the post. */
    newPost.formatEntry();
    newPost.setUserName(userName);
    newPost.readInput();
    newPost.getTimeDate();
    newPost.submitPost();
    newPost.freePostVariables();
    free(userName);
	return (0);
}