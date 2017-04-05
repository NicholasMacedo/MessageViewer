#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Opens the file and returns the pointer. Exits if fails. */
FILE * openFile(char * fileName) {
    FILE * filePointer = fopen(fileName,"r");
    if (filePointer == NULL) {
        printf("Cannot open ) the file. (Include file in command line.)\n");
        exit(1);
    }
    return(filePointer);
}

/* Gets the flags inbetween the commas. */
char * getNextFlag(char * flags, int * inCurrSpot){
    int i = 0;
    char * string = calloc(2048, sizeof(char));
    int stringPos = 0;

    for (i = 0; i < strlen(flags); ++i) {
    	if (i > (*inCurrSpot) || (*inCurrSpot) == 0) {
	    	if (flags[i] == '\"') {
	    		if (flags[i+1] == ',') {
	    			string[stringPos] = '\"';
	    			(*inCurrSpot) = i+1;
	    			return(string);
	    		} else {
	    			string[stringPos] = flags[i];
	    			stringPos ++;
	    		}
	    	} else if (flags[i] == ',') {
	    		(*inCurrSpot) = i;
	    		return(string);
	    	} else {
	    		string[stringPos] = flags[i];
	    		stringPos ++;
	    	}
	    }
    }
    (*inCurrSpot) = i;
    return(string);
}

/* Checks to see if the executable exists by trying to open it. */
int doesExist(char * filePath) {
    FILE * filePointer = fopen(filePath,"r");
    if (filePointer == NULL) {
        return(0);
    }
    fclose(filePointer);
    return(1);
}

/* Parses the flags and makes the HTML from the information */
void getFlags (char type, char * flags) {

    int currSpot = 0;
    char * currFlag;
    int hit = 0;
    
    /* Flag Variables */
    char * name = NULL; /* Button, */
    char * link = NULL; /* Button, Link */
    char * path = NULL; /* Executable, Picture, Text */
    char * text = NULL; /* Heading, Link, Text */
    char * value = NULL; /* Radio, Input */
    char ** tempArray = NULL;
    char * extra = calloc(2048, sizeof(char));
    int size; /* Heading */
    int height = 100; /* Picture */
    int width = 100; /* Picture */
    int found = 0;
    int arrayPos = 0;
    int i = 0;
    int numRadio = 0;

    switch(type){
        case 'b':
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 'n' && currFlag[1] == 'a' && currFlag[2] == 'm' && currFlag[3] == 'e') {
                    free(name);
                	name = calloc(2048,sizeof(char));
                	sscanf(currFlag,"name=\"%[^\n]",name);
                	name[strlen(name)-1] = '\0';
                } else if (currFlag[0] == 'l' && currFlag[1] == 'i' && currFlag[2] == 'n' && currFlag[3] == 'k') {
                    free(link);
                	link = calloc(2048,sizeof(char));
                	sscanf(currFlag,"link=\"%[^\n]",link);
                	link[strlen(link)-1] = '\0';
                } else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }

            printf("<form action=\"%s\" %s>\n    <input class=\"button\" name=\"%s\"type=\"submit\" value=\"%s\"/>\n</form>\n", link, extra, name, name); 

            free(name);
            free(extra);
            free(link);
            break;
        case 'd':
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                strcat(extra, " ");
                strcat(extra, currFlag);
                free(currFlag);
            }
        	printf("<hr%s>\n", extra);
            free(extra);
            break;
        case 'c':
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 'p' && currFlag[1] == 'o' && currFlag[2] == 's') {
                    sscanf(currFlag,"pos=%d", &size);
                }else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }
            if (size == 1) {
                printf("<div%s>\n",extra);
            } else {
                printf("</div>\n");
            }
            free(extra);
            break;
        case 'e':
            /* Check: Current Dir, /bin and then system bin file for executable. */
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 'e' && currFlag[1] == 'x' && currFlag[2] == 'e') {
                    free(path);
                	path = calloc(2048,sizeof(char));
                	sscanf(currFlag,"exe=\"%[^\n]",path);
                	path[strlen(path)-1] = '\0';
                	if (doesExist(path) == 1) {
                        char * temp = calloc(2048,sizeof(char));
                        strcat(temp,"./");
                        strcat(temp,path);
                        if (value == NULL) {
                            strcat(temp," >/dev/null");
                            system(temp);
                        } else {
                            strcat(temp,value);
                            system(temp);
                        }
                        found = 1;
                        free(temp);
                	}
                	char * newPath = calloc(2048,sizeof(char));
                	strcat(newPath, "bin/");
                	strcat(newPath,path);
                	if (doesExist(newPath) == 1 && found == 0) {
                        char * temp = calloc(2048,sizeof(char));
                        strcat(temp,"./");
                        strcat(temp,newPath);
                        strcat(temp," >/dev/null");
                        system(temp);
                        free(temp);
                	}
                	free(newPath);
                	newPath = calloc(2048,sizeof(char));
                	strcat(newPath, "/bin/");
                	strcat(newPath,path);
                	if (doesExist(newPath) == 1 && found == 0) {
                        strcat(newPath," >/dev/null");
                        system(newPath);
                	}
                	free(newPath);
                }
                free(currFlag);
            }
            free(path);
            free(value);
            free(extra);
            break;
        case 'h':
            text = calloc(2048,sizeof(char));
            strcpy(text,"HEADING");
            size = 3;

            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 's' && currFlag[1] == 'i' && currFlag[2] == 'z' && currFlag[3] == 'e') {
                	sscanf(currFlag,"size=%d", &size);
                }else if (currFlag[0] == 't' && currFlag[1] == 'e' && currFlag[2] == 'x' && currFlag[3] == 't') {
                	free(text);
                	text = calloc(2048,sizeof(char));
                	sscanf(currFlag,"text=\"%[^\n]",text);
                	text[strlen(text)-1] = '\0';
                } else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }
            printf("<h%d%s>%s</h%d>\n", size, extra, text, size);
            free(text);
            free(extra);
            break;
        case 'i':
            /* Uses the 2D array to store the iformation. Then prints when done. */
            tempArray = malloc(sizeof(char *)*50);
            for (i = 0; i < 50; ++i) {
                tempArray[i] = calloc(2048,sizeof(char));
            }

            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 'a' && currFlag[1] == 'c' && currFlag[2] == 't' && currFlag[3] == 'i' && currFlag[4] == 'o' && currFlag[5] == 'n') {
                	free(path);
                    path = calloc(2048,sizeof(char));
                	sscanf(currFlag,"action=\"%[^\n]",path);
                	path[strlen(path)-1] = '\0';
                }else if (currFlag[0] == 't' && currFlag[1] == 'e' && currFlag[2] == 'x' && currFlag[3] == 't') {

                	/*  Get the Text */
                	text = calloc(2048,sizeof(char));
                	sscanf(currFlag,"text=\"%[^\n]",text);
                	text[strlen(text)-1] = '\0';
                	free(currFlag);

                	/* Get the Name */
                	currFlag = getNextFlag(flags,&currSpot);
                	name = calloc(2048,sizeof(char));
                	sscanf(currFlag,"name=\"%[^\n]",name);
                	name[strlen(name)-1] = '\0';
                	free(currFlag);

                	/* Get the Value */
                	currFlag = getNextFlag(flags,&currSpot);
                	value = calloc(2048,sizeof(char));
                	sscanf(currFlag,"value=\"%[^\n]",value);
                	value[strlen(value)-1] = '\0';
                	sprintf(tempArray[arrayPos],"    %s:\n    <input type=\"text\" name=\"%s\" value=\"%s\" required><br>\n", text, name, value);
                    arrayPos ++;
                    free(text);
                    free(name);
                    free(value);
                }else if (currFlag[0] == 's' && currFlag[1] == 'e' && currFlag[2] == 'l' && currFlag[3] == 'e' && currFlag[4] == 'c' && currFlag[5] == 't') {
                    /* Get the Value */
                    value = calloc(2048,sizeof(char));
                    sscanf(currFlag,"select=\"%[^\n]",value);
                    value[strlen(value)-1] = '\0';
                    if (numRadio == 0) {
                        sprintf(tempArray[arrayPos],"    <input type=\"radio\" name=\"radio\" value=\"%s\" checked> %s <br>\n",value, value);
                        arrayPos ++;
                    } else {
                        sprintf(tempArray[arrayPos],"    <input type=\"radio\" name=\"radio\" value=\"%s\"> %s <br>\n",value, value);
                        arrayPos ++;
                    }
                    numRadio ++;
                    free(value);
                } else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }
            
            /* Prints the information in the 2D array to the screen and then frees the 2D array. */
            printf("<form action=\"%s\" method=\"POST\"%s>\n", path,extra);
            for (i = 0; i < 50; ++i) {
                printf("%s",tempArray[i]);
            }
            printf("    <input class=\"submitButton\" type=\"submit\" value=\"Submit\">\n</form>\n");
            
            for (i = 0; i < 50; ++i) {
                free(tempArray[i]);
            }
            free(tempArray);
            free(path);
            free(extra);
            break;
        case 'l':
            text = calloc(2048,sizeof(char));
            strcpy(text,"link");
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 't' && currFlag[1] == 'e' && currFlag[2] == 'x' && currFlag[3] == 't') {
                	sscanf(currFlag,"text=\"%[^\n]",text);
                	text[strlen(text)-1] = '\0';
                } else if (currFlag[0] == 'l' && currFlag[1] == 'i' && currFlag[2] == 'n' && currFlag[3] == 'k') {
                	free(link);
                    link = calloc(2048,sizeof(char));
                	sscanf(currFlag,"link=\"%[^\n]",link);
                	link[strlen(link)-1] = '\0';
                } else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }
            printf("<a href=\"%s\"%s>%s</a><br>\n", link, extra, text);
            free(text);
            free(extra);
            free(link);
            break;
        case 'p':
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 'i' && currFlag[1] == 'm' && currFlag[2] == 'a' && currFlag[3] == 'g' && currFlag[4] == 'e') {
                	free(path);
                    path = calloc(2048,sizeof(char));
                	sscanf(currFlag,"image=\"%[^\n]",path);
                	path[strlen(path)-1] = '\0';
                } else if (currFlag[0] == 's' && currFlag[1] == 'i' && currFlag[2] == 'z' && currFlag[3] == 'e') {
                	sscanf(currFlag,"size=%dx%d", &width, &height);
                } else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }
            printf("<img src=\"%s\" height=\"%d\" width=\"%d\" %s>\n", path, height, width, extra);
            free(path);
            free(extra);
            break;
        case 'r':
            /* Uses the 2D array to store the iformation. Then prints when done. */
            tempArray = malloc(sizeof(char *)*50);
            for (i = 0; i < 50; ++i) {
                tempArray[i] = calloc(2048,sizeof(char));
            }
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 'a' && currFlag[1] == 'c' && currFlag[2] == 't' && currFlag[3] == 'i' && currFlag[4] == 'o' && currFlag[5] == 'n') {
                    free(path);
                    path = calloc(2048,sizeof(char));
                    sscanf(currFlag,"action=\"%[^\n]",path);
                    path[strlen(path)-1] = '\0';
                }else if (currFlag[0] == 'n' && currFlag[1] == 'a' && currFlag[2] == 'm' && currFlag[3] == 'e') {
                    free(name);
                    name = calloc(2048,sizeof(char));
                    sscanf(currFlag,"name=\"%[^\n]",name);
                    name[strlen(name)-1] = '\0';
                }

                if (currFlag[0] == 'v' && currFlag[1] == 'a' && currFlag[2] == 'l' && currFlag[3] == 'u' && currFlag[4] == 'e') {
                    free(value);
                    value = calloc(2048,sizeof(char));
                    sscanf(currFlag,"value=\"%[^\n]",value);
                    value[strlen(value)-1] = '\0';
                    if (numRadio == 0) {
                        sprintf(tempArray[arrayPos],"<input type=\"radio\" name=\"%s\" value=\"%s\" checked> %s <br>\n", name, value, value);
                        arrayPos ++;
                    } else {
                        sprintf(tempArray[arrayPos],"<input type=\"radio\" name=\"%s\" value=\"%s\"> %s <br>\n", name, value, value);
                        arrayPos ++;
                    }
                    numRadio ++;
                } else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }
            
            /* Prints the 2D array to the screen and then frees the 2D array. */
            printf("<form action=\"%s\" method=\"POST\"%s>\n", path,extra);
            for (i = 0; i < 50; ++i) {
                printf("%s",tempArray[i]);
            }
            printf("    <input class=\"submitButton\" type=\"submit\" value=\"Submit\">\n</form>\n");
            
            for (i = 0; i < 50; ++i) {
                free(tempArray[i]);
            }
            free(tempArray);
            free(path);
            free(extra);

            free(name);
            free(value);
            break;
        case 't':
            /* Print the text after all flags have been parsed. Any extra info gets added to the front.  */
            text = calloc(2048,sizeof(char));
            strcpy(text,"Default text");
            while(currSpot < strlen(flags)){
                currFlag = getNextFlag(flags,&currSpot);
                if (currFlag[0] == 'f' && currFlag[1] == 'i' && currFlag[2] == 'l' && currFlag[3] == 'e') {
                	hit = 1;
                	free(path);
                    path = calloc(2048,sizeof(char));
                	sscanf(currFlag,"file=\"%[^\n]",path);
                	path[strlen(path)-1] = '\0';
                } else if (currFlag[0] == 't' && currFlag[1] == 'e' && currFlag[2] == 'x' && currFlag[3] == 't') {
                    free(text);
                	text = calloc(2048,sizeof(char));
                	sscanf(currFlag,"text=\"%[^\n]",text);
                	text[strlen(text)-1] = '\0';
                } else {
                    strcat(extra, " ");
                    strcat(extra, currFlag);
                }
                free(currFlag);
            }

            if (hit == 0) {
                if (strlen(extra) != 0) {
                    printf("<p%s>%s</p>\n", extra, text);
                } else {
            	    printf("%s\n",text);
                }
            } else {
                printf("%s\n",extra);
                FILE * filePointer = fopen(path,"r");
                if (filePointer != NULL) {
                    char * buffer = calloc(2048, sizeof(char));
                    while(fgets(buffer, 2048, filePointer)){
                        buffer[strlen(buffer)-1] = '\0';
                        printf("%s\n", buffer);
                        free(buffer);
                        buffer = calloc(2048,sizeof(char));
                    }
                    free(buffer);
                }
                fclose(filePointer);
            }
            free(path);
            free(extra);
            free(text);
            break;
    }
}

/* Gets each element from the fileLine */
void parseLine(char * element){
	char type = '~';
	char * flags = calloc(2048,sizeof(char));
	if (element[0] == '.' && element[2] == '(') {
		sscanf(element,".%c(%[^\n])",&type,flags);	
        flags[strlen(flags)-1] = '\0';
	    getFlags(type,flags);
	} else {
		printf("%s\n",element);
	}
	free(flags);
}


int main(int argc, char *argv[]) {
	if (argc != 2) {
        printf("Please run the code with the following format: ./a3 fileName\n");
        exit(1);
	}

    FILE * filePointer = openFile(argv[1]);
    char currChar;
    char prevChar = '\0';
	char * element =  calloc(2048, sizeof(char));
	int inBracket = 0;
	int currPos = 0;
	while((currChar = getc(filePointer)) != EOF){
		/* Checks to see if everything is read in. Sends to parse function if yes. */
		if (currChar == '.' && inBracket == 0 && strlen(element) != 0){
			parseLine(element);
			free(element);
			element =  calloc(509, sizeof(char));
			currPos = 0;
		/* Keep track of braces to check when inside element. */
		} else if (currChar == ')'){
			inBracket ++;
		} else if (currChar == '('){
			inBracket --;
		/* New Line means this line is done and push whatever has been read to parse function. */
		} else if (currChar == '\n' && strlen(element) != 0){
			parseLine(element);
			free(element);
			element =  calloc(509, sizeof(char));
			currPos = 0;
			inBracket = 0;
			continue;
		} else if (currChar == '\"'){
			element[currPos] = currChar;
			currPos ++;
			while(((currChar = getc(filePointer)) != ','  || currChar != ')' ) && prevChar != '\"'){
				element[currPos] = currChar;
				prevChar = currChar;
				currPos ++;
			}
			prevChar = '\0';
			if (currChar == ')') {
				ungetc(currChar,filePointer);
				currChar = '\0';
			}
		}

        /* Adds it to the string after all the checks. */
        if (currChar != '\0') {
			element[currPos] = currChar;
			currPos ++;
	    }
	}
    fclose(filePointer);
	free(element);
	return 0;
}