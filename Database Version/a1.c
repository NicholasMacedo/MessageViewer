#include "a1.h"

int main(int argc, char *argv[])
{
    item * programList;

    if (argc != 2) {
        printf("Please run the code with the following format: ./a1 fileName.cc\n");
        exit(1);
    }

    FILE * filePointer = openFile(argv[1]);

    programList = readFile(filePointer);
    fclose(filePointer);

    programList = replaceClass(programList);

    numFunctInClass(programList);
    programList =  changeFunctInClass (programList);

    renameFunctionsInCode(programList);
    replaceVarInClassFunct(programList);

    toFile(programList, argv[1]);

    freeList(programList);
    return 0;
}

/* Opens the file and closes if invalid file. */
FILE * openFile(char * fileName) {
    FILE * filePointer = fopen(fileName,"r");
    if (filePointer == NULL) {
        printf("Cannot open the file. (Include file in command line.)\n");
        exit(1);
    }
    return(filePointer);
}

/* Raplces all "Class" with "Struct" */
item * replaceClass(item * programList) {
    item * current = programList->nextNode;
    while (current != NULL) {
        if (strcmp(current->token, "class") == 0) {
            free(current->token);
            current->token = calloc(2048, sizeof(char));
            strcpy(current->token, "struct");
        }
        current = current->nextNode;
    }
    return(programList);
}

/* Loops through all classes and performs necessary changes. */
item * changeFunctInClass (item * programList) {
    item * current = programList->nextNode;
    item * currClassName = NULL;
    item * currFunctHead = NULL;
    item * currFunctName = NULL;
    item * classVarList = NULL;
    item * paramVarList = NULL;
    item * localVarList = NULL;
    item * newNode = NULL;
    item * afterFunctPTR = NULL;
    item * endBrace = NULL;
    char * newFunctName;
    char * currFunctType;
    item * functionsInClass = initNode();
    int inClass = 0;
    int hasOneFunct = 0;
    int inFunct = 0;
    int hitOnce = 0;
    while(current != NULL) {

        /* Class/Struct found */
        if (strcmp(current->token, "struct") == 0 && strcmp(current->nextNode->nextNode->token, "{") == 0) {
            inClass ++;
            currClassName = current->nextNode;

            /* Get list of all class variables*/
            if (classVarList != NULL) {
                freeList(classVarList);
            }
            classVarList = getClassVarList(currClassName->nextNode);

            current = current->nextNode->nextNode->nextNode;
            while(inClass != 0){
                if (strcmp(current->token,"{") == 0) {
                    inClass ++;
                } else if (strcmp(current->token,"}") == 0) {
                    inClass = inClass - 1;
                    if (inClass == 0) {
                        endBrace = current;
                    }
                } else {
                    /* Function inside Class/Struct found */
                    if (isVarType(current->token) == 1 && current->isFunction == 1) {
                        currFunctHead = current;

                        currFunctType = getFunctType(currFunctHead);
                        functionsInClass = addToBack(functionsInClass,0,0,0,currFunctType);

                        if (strcmp(current->token, "struct") == 0) {
                            current = current->nextNode;
                        }
                        while (strcmp(current->nextNode->token, "*") == 0) {
                            current = current->nextNode;
                        }
                        currFunctName = current->nextNode;
                        current = current->nextNode;
                       
                        hasOneFunct = 1;
                        /* Skip the contents of the function. Work outsourced to functions. */
                        functionsInClass = addToBack(functionsInClass,0,0,0,currFunctName->token);

                        /* Add Struct to the function */
                        current = current->nextNode;
                       
                        newNode = makeConstructorNode("struct", 0, 0);
                        current = insertNext(current,newNode);           
                        current = current->nextNode;
                        newNode = makeConstructorNode(currClassName->token, 0, 1);
                        current = insertNext(current,newNode);
                        current = current->nextNode;
                        newNode = makeConstructorNode("*", 0, 1);
                        current = insertNext(current,newNode);
                        current = current->nextNode;
                        newNode = makeConstructorNode("sPtrNM", 0, 1);
                        current = insertNext(current,newNode);
                        current = current->nextNode;
                        
                        if (strcmp(current->nextNode->token,")") != 0) {
                            newNode = makeConstructorNode(",", 0, 0);
                            current = insertNext(current,newNode);                     
                            current = current->nextNode;
                        }

                        do {
                            current = current->nextNode;
                            
                            if (strcmp(current->token,"{") == 0) {
                                hitOnce = 1;
                                inFunct ++;
                            } else if (strcmp(current->token,"}") == 0) {
                                inFunct = inFunct - 1;
                            }
                        } while(inFunct != 0 || hitOnce != 1);
                        hitOnce = 0;
                        inFunct = 0;

                        if (paramVarList != NULL) {
                            freeList(paramVarList);
                        }
                        paramVarList = getParamVarList(currFunctName);

                        if (localVarList != NULL) {
                            freeList(localVarList);
                        }
                        /* Perform all these changes on the function. */
                        localVarList = getLocalVarList(currFunctName);
                        int numVars = classVarInFunct(currFunctName, classVarList, paramVarList, localVarList, currFunctName->token);
                        if (numVars < 0) {
                            printf("ERROR.\n");
                        }
                        newFunctName = getNewFunctName(currFunctName, currClassName->token);
                        functionsInClass = addToBack(functionsInClass,0,0,0,newFunctName);
                        free(currFunctName->token);
                        currFunctName->token = newFunctName;
                        free(currFunctType);
                    }
                }
                current = current->nextNode;
            }
        
            /* If there are functions in the class, insert pointers and make a constructor.*/
            if (functionsInClass->nextNode != NULL) {
                currClassName = insertFunctPointer(functionsInClass, currClassName, &afterFunctPTR);
                current = makeConstructor( functionsInClass, current, currClassName->token);
            }

            /* Add constructors to all instances of the struct. */
            if (hasOneFunct == 1) {
                addConstructor(programList,currClassName->token);
            }

            /* move the bracket if there is functions */
            if (afterFunctPTR != NULL) {
                newNode = makeConstructorNode("}", endBrace->numNewLines, endBrace->numWhiteSpace);
                afterFunctPTR = insertNext(afterFunctPTR,newNode);
                afterFunctPTR = afterFunctPTR->nextNode;
                newNode = makeConstructorNode(";", 0, 0);
                afterFunctPTR = insertNext(afterFunctPTR,newNode);
                afterFunctPTR = afterFunctPTR->nextNode;
                endBrace->token[0] = '\0';
                endBrace->nextNode->token[0] = '\0';
                afterFunctPTR = NULL;
            }
            hasOneFunct = 0;
            freeList(functionsInClass);
            functionsInClass = initNode();
        }
        current = current->nextNode;
    }

    if (classVarList != NULL) {
        freeList(classVarList);
    }
    if (paramVarList != NULL) {
        freeList(paramVarList);
    }
    if (localVarList != NULL) {
        freeList(localVarList);
    }
    if (functionsInClass != NULL) {
        freeList(functionsInClass);
    }
    return (programList);
}

/* Flags functions inside classes */
void numFunctInClass (item * programList) {
    int inClass = 0;
    item * current = programList->nextNode;
    item * foundStruct = NULL;
    item * functionVar = NULL;

    while (current != NULL) {
        if (strcmp(current->token,"struct") == 0) {
            foundStruct = (current->nextNode)->nextNode; /* Skips the function / variable name. */
            if (strcmp(foundStruct->token,"{") == 0) {
                current = (current->nextNode)->nextNode;
            }
        }

        if (strcmp(current->token,"{") == 0) {
            inClass ++;
        }

        if (isVarType(current->token) == 1 && inClass == 1) {
            functionVar = current;
            if (strcmp(current->token, "struct") == 0) {
                current = current->nextNode;
            }
            while (strcmp((current->nextNode)->token, "*") == 0) {
                current = current->nextNode;
            }
            current = (current->nextNode)->nextNode; /* Skips the function / variable name. */
            if (strcmp(current->token,"(") == 0) {
                functionVar->isFunction = 1;
            }
        }

        if (strcmp(current->token,"}") == 0) {
            inClass --;
        }
        current = current->nextNode;
    }

    return;
}

/* Returns 1 if variable else 0 */
int isVarType(char * type) {
    if (strcmp(type,"int") == 0 || strcmp(type,"char") == 0 || strcmp(type,"long") == 0 || strcmp(type,"short") == 0 || strcmp(type,"double") == 0 || strcmp(type,"void") == 0 || strcmp(type,"struct") == 0 || strcmp(type,"float") == 0) {
        return (1);
    }
    return (0);
}

/* Reads from the file and places into list. Uses punctuation and whitespace to break tokens. */
item * readFile(FILE * filePointer) {
    int arrayPos = 0;
    int numWhiteSpace = 0;
    int numNewLines = 0;
    int numTabs = 0;
    int inQuotes = -1;
    char currChar;
    item * programList = initNode();
    char * temp = calloc(2048, sizeof(char));
    while((currChar = fgetc(filePointer)) != EOF){
        if (isWhiteSpace(currChar) == 1) {
            if (temp[0] == '/' && temp[1] == '/' && currChar != '\n') { /* Skip if in // quote */
                temp[arrayPos] = currChar;
                arrayPos ++;
            } else if (temp[0] == '/' && temp[1] == '*') { /* Checks to see if the comment is done */
                if (temp[arrayPos-1] == '/' && temp[arrayPos-2] == '*') {
                    programList = addToBack(programList, numWhiteSpace, numTabs, numNewLines, temp);
                    free(temp);
                    numWhiteSpace = 0;
                    numTabs = 0;
                    numNewLines = 0;
                    arrayPos = 0;
                    temp = calloc(2048, sizeof(char));
                    if (currChar == ' ') {
                        numWhiteSpace ++;
                    } else if (currChar == '\t') {
                        numTabs ++;
                    } else if (currChar == '\n' || currChar == '\r') {
                        numNewLines ++;
                    }
                } else {
                    temp[arrayPos] = currChar;
                    arrayPos ++;
                }
            } else if (inQuotes == 1) {
                temp[arrayPos] = currChar;
                arrayPos ++;
            } else {
                if (strlen(temp) != 0) {
                    programList = addToBack(programList, numWhiteSpace, numTabs, numNewLines, temp);
                    free(temp);
                    numWhiteSpace = 0;
                    numTabs = 0;
                    numNewLines = 0;
                    arrayPos = 0;
                    temp = calloc(2048, sizeof(char));
                    if (currChar == ' ') {
                        numWhiteSpace ++;
                    } else if (currChar == '\t') {
                        numTabs ++;
                    } else if (currChar == '\n' || currChar == '\r') {
                        numNewLines ++;
                    }
                } else {
                    if (currChar == ' ') {
                        numWhiteSpace ++;
                    } else if (currChar == '\t') {
                        numTabs ++;
                    } else if (currChar == '\n' || currChar == '\r') {
                        numNewLines ++;
                    }
                }
            }
        } else if (isPunctuation(currChar) == 1) {
            if (currChar == '\"') {
                if (arrayPos == 0) {
                    inQuotes = inQuotes * -1;
                } else if (temp[arrayPos-1] != '\\') {
                    inQuotes = inQuotes * -1;
                } 
                temp[arrayPos] = currChar;
                arrayPos ++;
                continue;
            } if (temp[0] == '/' && temp[1] == '/') {
                temp[arrayPos] = currChar;
                arrayPos ++;
            } else if (temp[0] == '/' && temp[1] == '*') {
                if (temp[arrayPos-1] == '/' && temp[arrayPos-2] == '*') {
                    programList = addToBack(programList, numWhiteSpace, numTabs, numNewLines, temp);
                    free(temp);
                    numWhiteSpace = 0;
                    numTabs = 0;
                    numNewLines = 0;
                    arrayPos = 0;
                    temp = calloc(2048, sizeof(char));
                } else {
                    temp[arrayPos] = currChar;
                    arrayPos ++;
                }
            }else if (inQuotes == 1 && currChar != '\"') {
                    temp[arrayPos] = currChar;
                    arrayPos ++;
            }else {
                if (strlen(temp) != 0) {
                    programList = addToBack(programList, numWhiteSpace,  numTabs, numNewLines, temp);
                    free(temp);
                    numWhiteSpace = 0;
                    numTabs = 0;
                    numNewLines = 0;
                    arrayPos = 0;
                    temp = calloc(2048, sizeof(char));
                }
                temp[0] = currChar;
                programList = addToBack(programList, numWhiteSpace,  numTabs, numNewLines, temp);
                free(temp);
                numWhiteSpace = 0;
                numTabs = 0;
                numNewLines = 0;
                arrayPos = 0;
                temp = calloc(2048, sizeof(char));
            }
        } else if (temp[0] != '/' && currChar == '*') { /* Checks if multiplication */
            if (strlen(temp) != 0) {
                    programList = addToBack(programList, numWhiteSpace,  numTabs, numNewLines, temp);
                    free(temp);
                    numWhiteSpace = 0;
                    numTabs = 0;
                    numNewLines = 0;
                    arrayPos = 0;
                    temp = calloc(2048, sizeof(char));
            }
            temp[0] = currChar;
            programList = addToBack(programList, numWhiteSpace,  numTabs, numNewLines, temp);
            free(temp);
            numWhiteSpace = 0;
            numTabs = 0;
            numNewLines = 0;
            arrayPos = 0;
            temp = calloc(2048, sizeof(char));
        } else if (temp[0] == '/' && !(currChar == '*' || currChar == '/') && !(temp[1] == '*' || temp[1] == '/')) { /* Check for division */
            if (strlen(temp) != 0) {
                programList = addToBack(programList, numWhiteSpace,  numTabs, numNewLines, temp);
                free(temp);
                numWhiteSpace = 0;
                numTabs = 0;
                numNewLines = 0;
                arrayPos = 0;
                temp = calloc(2048, sizeof(char));
            }
            temp[0] = currChar;
            programList = addToBack(programList, numWhiteSpace,  numTabs, numNewLines, temp);
            free(temp);
            numWhiteSpace = 0;
            numTabs = 0;
            numNewLines = 0;
            arrayPos = 0;
            temp = calloc(2048, sizeof(char));
        } else {
            temp[arrayPos] = currChar;
            arrayPos ++;
        }
    }
    free(temp);
    return(programList);
}

/* Returns 1 if white space. Else 0. */
int isWhiteSpace (char currChar) {
    if (currChar == ' ' || currChar == '\t' || currChar == '\n' || currChar == '\r') {
        return (1);
    }
    return (0);
}

/* Returns 1 if punctuation. Else 0. */
int isPunctuation (char currChar) {
    if (currChar == ';' || currChar == '{' || currChar == '}' || currChar == '(' || currChar == ')' || currChar == ','|| currChar == '+' || currChar == '-' || currChar == '=' || currChar == '\"' || currChar == '.'|| currChar == '[' || currChar == ']' || currChar == '&') {
        return (1);
    }
    return (0);
}

/* Lists all class variables */
item * getClassVarList(item * programList) {
    item * classVarList = initNode();
    item * current = programList;
    int inClass = 0;
    int inParam = 0;
    while (current != NULL) {
        if (strcmp(current->token,"{") == 0) {
            inClass = inClass + 1;
        } else if (strcmp(current->token,"}") == 0) {
            inClass = inClass - 1;
        } else if (strcmp(current->token,")") == 0) {
            inParam = 0;
        } else{
            if (isVarType(current->token) == 1 && inParam != 1 && inClass == 1) {
                if (strcmp(current->token,"struct") == 0) {
                    if (strcmp(current->nextNode->nextNode->token,"{") == 0) {
                        current = current->nextNode->nextNode;
                        continue;
                    }
                    current = current->nextNode;
                }

                current = current->nextNode;
                while (strcmp(current->token, "*") == 0) {
                    current = current->nextNode;
                }
                if (strcmp(current->nextNode->token, "(") != 0) {
                    if (isPunctuation(current->token[0]) == 0) {
                        classVarList = addToBack(classVarList,0,0,0,current->token);
                    }

                    while(strcmp(current->nextNode->token, ",") == 0){
                        current = current->nextNode->nextNode;
                        if (isPunctuation(current->token[0]) == 0) {
                            classVarList = addToBack(classVarList,0,0,0,current->token);
                        }
                    }
                } else {
                    inParam = 1;
                }
            }
        }

        if (inClass == 0) {
            return(classVarList);
        }
        current = current->nextNode;
    }
    return(classVarList);
}

/* Puts the name of all Global Variables in a list */
item * getGlobalVarList(item * programList) {
    item * globalVarList = initNode();
    item * current = programList->nextNode;
    int isGlobal = 0;
    int inParam = 0;

    while(current != NULL) {
        if (strcmp(current->token,"{") == 0) {
            isGlobal ++;
        }

        if (strcmp(current->token,"}") == 0) {
            isGlobal = isGlobal - 1;
        }

        if (strcmp(current->token,")") == 0) {
            inParam = 0;
        }

        if (isVarType(current->token) == 1 && isGlobal == 0 && inParam == 0) {

            if (strcmp(current->token,"struct") == 0) {
                if (strcmp(current->nextNode->nextNode->token,"{") == 0) {
                    current = current->nextNode->nextNode;
                    continue;
                }
                current = current->nextNode;
            }

            current = current->nextNode;
            while (strcmp(current->token, "*") == 0) {
                current = current->nextNode;
            }
            if (strcmp(current->nextNode->token, "(") != 0) {
                if (isPunctuation(current->token[0]) == 0) {
                    globalVarList = addToBack(globalVarList,0,0,0,current->token);
                }

                while(strcmp(current->nextNode->token, ",") == 0) {
                    current = current->nextNode->nextNode;
                    if (isPunctuation(current->token[0]) == 0) {
                        globalVarList = addToBack(globalVarList,0,0,0,current->token);
                    }
                }
            } else {
                inParam = 1;
            }
        }

        current = current->nextNode;
    }
    printf("***PRINTING GLOBAL VAR LIST:***\n");
    printList(globalVarList);
    return(globalVarList);
}

/* Gets list of param variables */
item * getParamVarList(item * programList){
    item * paramVarList = initNode();
    item * current = programList->nextNode;
    int inParam = 0;
    while(current != NULL){
        if (strcmp(current->token,"(") == 0) {
            inParam = 1;
        } else if (strcmp(current->token,")") == 0) {
            inParam = 0;
        } else if (strcmp(current->token,"{") == 0) {
            return(paramVarList);
        } else if (inParam == 1) {
            if (isVarType(current->token) == 1) {
                if (strcmp(current->token,"struct") == 0) {
                    current = current->nextNode;
                }
                current = current->nextNode;
                while (strcmp(current->token, "*") == 0) {
                    current = current->nextNode;
                }

                if (isPunctuation(current->token[0]) == 0) {
                        paramVarList = addToBack(paramVarList,0,0,0,current->token);
                }

                while(strcmp(current->nextNode->token, ",") == 0){
                    current = current->nextNode->nextNode;
                    if (strcmp(current->token,"struct") == 0) {
                        current = current->nextNode->nextNode;
                    } else {
                        current = current->nextNode;
                    }
                    while (strcmp(current->token, "*") == 0) {
                        current = current->nextNode;
                    }
                    if (isPunctuation(current->token[0]) == 0) {
                        paramVarList = addToBack(paramVarList,0,0,0,current->token);
                    }
                }
            }
        }
        current = current->nextNode;
    }
    return(paramVarList);
}

/* Get list of local variables */
item * getLocalVarList(item * programList) {
    item * localVarList = initNode();
    item * current = programList;
    int inFunct = 0;
    int hitOnce = 0;
    while (current != NULL) {
        if (strcmp(current->token,"{") == 0) {
            hitOnce = 1;
            inFunct = inFunct + 1;
        } else if (strcmp(current->token,"}") == 0) {
            inFunct = inFunct - 1;
        } else{
            if (isVarType(current->token) == 1  && inFunct > 0) {
                if (strcmp(current->token,"struct") == 0) {
                    if (strcmp(current->nextNode->nextNode->token,"{") == 0) {
                        current = current->nextNode->nextNode;
                        continue;
                    }
                    current = current->nextNode;
                }

                current = current->nextNode;
                while (strcmp(current->token, "*") == 0) {
                    current = current->nextNode;
                }
                if (strcmp(current->nextNode->token, "(") != 0) {
                    if (isPunctuation(current->token[0]) == 0) {
                        localVarList = addToBack(localVarList,0,0,0,current->token);
                    }

                    while(strcmp(current->nextNode->token, ",") == 0){
                        current = current->nextNode->nextNode;
                        if (isPunctuation(current->token[0]) == 0) {
                            localVarList = addToBack(localVarList,0,0,0,current->token);
                        }
                    }
                }
            }
        }
        if (inFunct == 0 && hitOnce == 1) {
            return(localVarList);   
        }
        current = current->nextNode;
    }
    return(localVarList);   
}

/* Prints to the file */
void toFile(item * programList, char * inFileName) {
    item * current = programList->nextNode;
    char * fileName = malloc(sizeof(char)* (strlen(inFileName) + 3));
    inFileName[strlen(inFileName)-3] = '\0';
    strcpy(fileName,inFileName);
    strcat(fileName,".c");
    FILE * filePointer = fopen(fileName,"w");
    int i = 0;
    if (filePointer == NULL) {
        printf("Error creating file. Exiting.\n");
        exit(1);
    }

    while (current != NULL) {
        for (i = 0; i < current->numNewLines; ++i) { /* Prints \n before tab and space because \n is more prominant */
            fprintf(filePointer, "\n");
        }
        
        for (i = 0; i < current->numTabs; ++i) {
            fprintf(filePointer, "\t");
        }

        for (i = 0; i < current->numWhiteSpace; ++i) {
            fprintf(filePointer, " ");
        }

        fprintf(filePointer, "%s", current->token);
        current = current->nextNode;
    }
    fclose(filePointer);
    free(fileName);
    return;
}

/* sets inClassVar=1 if its a class variable. */
int classVarInFunct(item * programList, item * classVarList, item * paramVarList, item * localVarList, char * functionName) {
    item * current = programList->nextNode;
    int numVars = 0;
    int inFunct = 0;
    int hitOnce = 0;
        do {
            current = current->nextNode;
            if (strcmp(current->token,"{") == 0) {
                hitOnce = 1;
                inFunct ++;
            } else if (strcmp(current->token,"}") == 0) {
                inFunct = inFunct - 1;
            } else {
                if (isInList(classVarList, current->token) == 1) { /* If its in the class var list and NOT in the local or params then its class var. */
                    if (isInList(localVarList, current->token) != 1 && isInList(paramVarList, current->token) != 1) {
                        current->isClassVar = 1;
                        numVars ++;
                    } 
                }
            }
        } while(inFunct != 0 || hitOnce != 1);
    
    return(numVars);
}

/* Creates the new function name using the class name and looping through the parameters. */
char * getNewFunctName(item * functionName, char * currClassName) {
    char * newFunctName = calloc(2048,sizeof(char));
    item * current = functionName->nextNode->nextNode;
    char * paramType = calloc(2,sizeof(char));
    int inParam = 1;
    strcpy(newFunctName, currClassName);
    strcat(newFunctName, functionName->token);
    while(inParam == 1) {
        if (strcmp(current->token, "(") == 0) {
            inParam ++;
        } else if (strcmp(current->token, ")") == 0) {
            inParam --;
        } else if (isVarType(current->token) == 1) {
            paramType[0] = current->token[0];
            strcat(newFunctName,paramType);
        } else {

        }
        current = current->nextNode;
    }
    free(paramType);
    return(newFunctName);
}

/* Returns function type */
char * getFunctType(item * currFunctHead) {
    char * currFunctType = calloc(2048, sizeof(char));
    item * current = currFunctHead;
    strcpy(currFunctType,current->token);

    if(strcmp(current->token,"struct") == 0) {
        strcat(currFunctType," ");
        strcat(currFunctType,current->nextNode->token);
        current = current->nextNode->nextNode;
    } else {
        current = current->nextNode;
    }

    while(current != NULL) {
        if (strcmp(current->token,"*") != 0) {
            return(currFunctType);
        } else {
            strcat(currFunctType,current->token);
        }
        current = current->nextNode;
    }
    strcat(currFunctType," ");
    return(currFunctType);
}

/* Inserts the function poitners for functions found in the class. */
item * insertFunctPointer(item * functionsInClass, item * currClassName, item ** afterFunctPTR) {
    item * current = currClassName->nextNode;
    item * currentFunctList = functionsInClass->nextNode;
    item * newNode = NULL;
    item * paramVarList = NULL;
    item * currParam = NULL;
    int isType = 0;

    while(current->nextNode->isFunction != 1) {
        current = current->nextNode;
    }
    while(currentFunctList != NULL) {
        if (isType == 0) {
            isType = 1;
            newNode = makeConstructorNode(currentFunctList->token, 1, 4);
            current = insertNext(current,newNode);           
            current = current->nextNode;
            currentFunctList = currentFunctList->nextNode->nextNode;
        } else {
            isType = 0;
            newNode = makeConstructorNode("(", 0, 1);
            current = insertNext(current,newNode);
            current = current->nextNode;
            newNode = makeConstructorNode("*", 0, 0);
            current = insertNext(current,newNode);
            current = current->nextNode;
            newNode = makeConstructorNode(currentFunctList->token, 0, 0);
            current = insertNext(current,newNode);
            current = current->nextNode;
            newNode = makeConstructorNode(")", 0, 0);
            current = insertNext(current,newNode);
            current = current->nextNode;
            newNode = makeConstructorNode("(", 0, 1);
            current = insertNext(current,newNode);
            current = current->nextNode;

            paramVarList = getFunctParam(current, currentFunctList->token);
            if (paramVarList != NULL) {
                currParam = paramVarList->nextNode;
                while(currParam != NULL) {
                    newNode = makeConstructorNode(currParam->token, 0, 0);
                    current = insertNext(current,newNode);
                    current = current->nextNode;

                    if (strcmp(currParam->token, "struct") == 0) {
                        currParam = currParam->nextNode;
                        newNode = makeConstructorNode(currParam->token, 0, 1);
                        current = insertNext(current,newNode);
                        current = current->nextNode;
                    }

                    currParam = currParam->nextNode;
                }
                freeList(paramVarList);
            }

            newNode = makeConstructorNode(")", 0, 0);
            current = insertNext(current,newNode);
            current = current->nextNode;
            newNode = makeConstructorNode(";", 0, 0);
            current = insertNext(current,newNode);
            current = current->nextNode;
            currentFunctList = currentFunctList->nextNode;
        }
    }
    (*afterFunctPTR) = current;
    return(currClassName);
}

item * getFunctParam(item * programList, char * functionName) {
    item * current = programList;
    item * paramVarList = NULL;
    int inParam = 0;
    while(current != NULL) {
        if (strcmp(current->token, functionName) == 0 && strcmp(current->nextNode->token, "(") == 0) {
            current = current->nextNode;
            if (strcmp(current->nextNode->token, ")") == 0) {
                return(paramVarList);
            } else {
                paramVarList = initNode();
                inParam = 1;
                current = current->nextNode;
                while (inParam > 0) {
                    if (strcmp(current->token, "(") == 0) {
                        inParam ++;
                    } else if(strcmp(current->token, ")") == 0) {
                        inParam --;
                    } else if (isVarType(current->token) == 1 || strcmp(current->token, "*") == 0 || strcmp(current->token, ",") == 0|| strcmp(current->token, "[") == 0|| strcmp(current->token, "]") == 0) {
                        if (strcmp(current->token, "struct") == 0) {
                            paramVarList = addToBack(paramVarList,0,0,0,current->token);
                            current = current->nextNode;
                            paramVarList = addToBack(paramVarList,0,0,0,current->token);
                        } else {
                            paramVarList = addToBack(paramVarList,0,0,0,current->token);
                        }
                    }
                    current = current->nextNode;
                }
                return(paramVarList);
            }
        }
        current = current->nextNode;
    }
    return(paramVarList);
}

item * makeConstructor(item * functionsInClass, item * endOfClass, char * currClassName) {
    item * currentFunctList = functionsInClass->nextNode->nextNode->nextNode;
    item * current = endOfClass;
    item * newNode = NULL;
    char * constructorName = calloc(2048, sizeof(char));
    char * functionPointer = NULL;
    
    newNode = makeConstructorNode("void", 2, 0);
    current = insertNext(current,newNode);
    current = current->nextNode;

    strcpy(constructorName,"constructor");
    strcat(constructorName, currClassName);
    newNode = makeConstructorNode(constructorName, 0, 1);
    current = insertNext(current,newNode);
    current = current->nextNode;

    newNode = makeConstructorNode("(", 0, 1);
    current = insertNext(current,newNode);
    current = current->nextNode;

    newNode = makeConstructorNode("struct", 0, 0);
    current = insertNext(current,newNode);
    current = current->nextNode;

    newNode = makeConstructorNode(currClassName, 0, 1);
    current = insertNext(current,newNode);
    current = current->nextNode;

    newNode = makeConstructorNode("*", 0, 1);
    current = insertNext(current,newNode);
    current = current->nextNode;

    newNode = makeConstructorNode("sPtrNM", 0, 1);
    current = insertNext(current,newNode);
    current = current->nextNode;

    newNode = makeConstructorNode(")", 0, 0);
    current = insertNext(current,newNode);
    current = current->nextNode;

    newNode = makeConstructorNode("{", 0, 1);
    current = insertNext(current,newNode);
    current = current->nextNode;

    while (currentFunctList != NULL) {
        newNode = makeConstructorNode("sPtrNM->", 1, 4);
        current = insertNext(current,newNode);
        current = current->nextNode;

        newNode = makeConstructorNode(currentFunctList->token, 0, 0);
        current = insertNext(current,newNode);
        current = current->nextNode;

        newNode = makeConstructorNode("=", 0, 1);
        current = insertNext(current,newNode);
        current = current->nextNode;

        if (functionPointer != NULL) {
            free(functionPointer);
        }

        functionPointer = calloc(2048, sizeof(char));
        strcpy(functionPointer,"&");
        strcat(functionPointer,currentFunctList->token);

        newNode = makeConstructorNode(functionPointer, 0, 1);
        current = insertNext(current,newNode);
        current = current->nextNode;

        newNode = makeConstructorNode(";", 0, 0);
        current = insertNext(current,newNode);
        current = current->nextNode;

        if (currentFunctList->nextNode != NULL) {
             currentFunctList = currentFunctList->nextNode->nextNode->nextNode;
        } else {
            currentFunctList = currentFunctList->nextNode;
        }
    }

    newNode = makeConstructorNode("}", 1, 0);
    current = insertNext(current,newNode);
    current = current->nextNode;
    free(functionPointer);
    free(constructorName);
    return(current);
}

void addConstructor(item * programList, char * structType) {
    item * current = programList->nextNode;
    item * varName = NULL;
    item * newNode = NULL;
    int isGlobal = 0;
    int foundMain = 0;
    char * constructorName = calloc(2048, sizeof(char));
    strcpy(constructorName,"constructor");
    strcat(constructorName, structType);
    item * constructorList = NULL;
    item * currConstructorNode = NULL;
    item * globalConstructorList = NULL;
    while(current != NULL) {
        if (strcmp(current->token, "{") == 0) {
            isGlobal ++;
        } else if (strcmp(current->token, "}") == 0) {
            isGlobal --;
        } else if (isGlobal == 0 && strcmp(current->token, "struct") == 0 && foundMain == 0) {
            if (strcmp(current->token,"struct") == 0) {
                varName = current;
                if (strcmp(current->nextNode->token,structType) == 0) {
                    if ((strcmp(current->nextNode->nextNode->nextNode->token,";") == 0 || strcmp(current->nextNode->nextNode->nextNode->token,",") == 0) && strcmp(current->nextNode->nextNode->token,"*") != 0) {
                        current = current->nextNode->nextNode;
                        globalConstructorList = initNode();
                        if (strcmp(current->nextNode->token, ",") == 0) {
                            globalConstructorList = addToBack(globalConstructorList,0,0,0,current->token);
                            while(strcmp(current->nextNode->token, ",") == 0) {
                                current = current->nextNode->nextNode;
                                globalConstructorList = addToBack(globalConstructorList,0,0,0,current->token);
                            }
                        } else {
                            globalConstructorList = addToBack(globalConstructorList,0,0,0,current->token);
                        }
                    }
                }
            }
            varName = NULL;
        } else if (strcmp(current->token,"main") == 0 && isFunction(current) == 1 && globalConstructorList != NULL) {
            varName = current;
            foundMain = 1;
            while(strcmp(varName->token,"{") != 0) {
                varName = varName->nextNode;
            }
            currConstructorNode = globalConstructorList->nextNode;
            while (currConstructorNode != NULL) {
                newNode = makeConstructorNode(constructorName, 1, varName->numWhiteSpace);
                varName = insertNext(varName,newNode);
                varName = varName->nextNode;
                newNode = makeConstructorNode("(", 0, 0);
                varName = insertNext(varName,newNode);
                varName = varName->nextNode;
                newNode = makeConstructorNode("&", 0, 0);
                varName = insertNext(varName,newNode);
                varName = varName->nextNode;
                newNode = makeConstructorNode(currConstructorNode->token, 0, 0);
                varName = insertNext(varName,newNode);
                varName = varName->nextNode;
                newNode = makeConstructorNode(")", 0, 0);
                varName = insertNext(varName,newNode);
                varName = varName->nextNode;
                newNode = makeConstructorNode(";", 0, 0);
                varName = insertNext(varName,newNode);
                varName = varName->nextNode;
                varName->nextNode->numNewLines ++;
                currConstructorNode = currConstructorNode->nextNode;
            }
            freeList(globalConstructorList);
        } else {
            if (strcmp(current->token,"struct") == 0) {
                varName = current;
                if (strcmp(current->nextNode->token,structType) == 0) {
                    if ((strcmp(current->nextNode->nextNode->nextNode->token,";") == 0 || strcmp(current->nextNode->nextNode->nextNode->token,",") == 0) && strcmp(current->nextNode->nextNode->token,"*") != 0) {
                        current = current->nextNode->nextNode;
                        constructorList = initNode();
                        if (strcmp(current->nextNode->token, ",") == 0) {
                            constructorList = addToBack(constructorList,0,0,0,current->token);
                           while(strcmp(current->nextNode->token, ",") == 0) {
                                current = current->nextNode->nextNode;
                                constructorList = addToBack(constructorList,0,0,0,current->token);
                            }
                        } else {
                            constructorList = addToBack(constructorList,0,0,0,current->token);
                        }
                        current = current->nextNode;
                        currConstructorNode = constructorList->nextNode;
                        while (currConstructorNode != NULL) {
                            newNode = makeConstructorNode(constructorName, 1, varName->numWhiteSpace);
                            current = insertNext(current,newNode);
                            current = current->nextNode;
                            newNode = makeConstructorNode("(", 0, 0);
                            current = insertNext(current,newNode);
                            current = current->nextNode;
                            newNode = makeConstructorNode("&", 0, 0);
                            current = insertNext(current,newNode);
                            current = current->nextNode;
                            newNode = makeConstructorNode(currConstructorNode->token, 0, 0);
                            current = insertNext(current,newNode);
                            current = current->nextNode;
                            newNode = makeConstructorNode(")", 0, 0);
                            current = insertNext(current,newNode);
                            current = current->nextNode;
                            newNode = makeConstructorNode(";", 0, 0);
                            current = insertNext(current,newNode);
                            current = current->nextNode;
                            current->nextNode->numNewLines ++;
                            currConstructorNode = currConstructorNode->nextNode;
                        }
                        freeList(constructorList);
                    }
                }
            }            
        }
        current = current->nextNode;
    }
    free(constructorName);
}

item * makeConstructorNode(char * inStr,int numNewLines, int numWhiteSpace) {
    item * newNode = initNode();
    newNode->token = calloc(2048, sizeof(char));
    newNode->numNewLines = numNewLines;
    newNode->numWhiteSpace = numWhiteSpace;
    strcpy(newNode->token, inStr);
    return(newNode);
}

void renameFunctionsInCode(item * programList) {
    item * current = programList->nextNode;
    item * functionName = NULL;
    item * varInFunct = NULL;
    item * beforeDot = NULL;
    char * newFunctName = NULL;
    item * newNode = NULL;
    int inFunct = 0;
    int hitOnce = 0;
    int inFunctVars = 0;

    while(current != NULL) {
        if (isVarType(current->token) == 1) {
            if (strcmp(current->token, "struct") == 0) {
                current = current->nextNode;
            }
            while (strcmp((current->nextNode)->token, "*") == 0) {
                current = current->nextNode;
            }
            functionName = current->nextNode;
            current = (current->nextNode)->nextNode; /* Skips the function / variable name. */
            if (strcmp(current->token,"(") == 0 && isFunction(functionName) == 1) {
                varInFunct = listVarInFunct(functionName, programList);

                do {
                    current = current->nextNode;
                    if (strcmp(current->token,"{") == 0) {
                        hitOnce = 1;
                        inFunct ++;
                    } else if (strcmp(current->token,"}") == 0) {
                        inFunct = inFunct - 1;
                    } else {
                        if (strcmp(current->token,".") == 0) {
                            if (strcmp(current->nextNode->nextNode->token,"(") == 0) {
                                if (newFunctName != NULL) {
                                    free(newFunctName);
                                }
                                current = current->nextNode;
                                newFunctName = calloc(2048,sizeof(char));
                                strcpy(newFunctName, getStructName(beforeDot->token, varInFunct));
                                strcat(newFunctName, current->token);
                                strcat(newFunctName, "s");
                                current = current->nextNode; /* the bracket */
                                newNode = makeConstructorNode("&", 0, 0);
                                current = insertNext(current,newNode);           
                                current = current->nextNode;
                                newNode = makeConstructorNode(beforeDot->token, 0, 0);
                                current = insertNext(current,newNode);
                                current = current->nextNode; 
                                if (strcmp(current->nextNode->token,")") != 0) {
                                    newNode = makeConstructorNode(",", 0, 0);
                                    current = insertNext(current,newNode);                     
                                    current = current->nextNode;
                                }
                                current = current->nextNode; /* first Param */
                                inFunctVars = 1;
                                while(inFunctVars != 0) {
                                    if (strcmp(current->token, ")") == 0) {
                                        inFunctVars --;
                                    } else if (strcmp(current->token, "(") == 0) {
                                        inFunctVars ++;
                                    } else {
                                        if (isPunctuation(current->token[0]) == 0) {
                                            newFunctName[strlen(newFunctName)] = getVarType(current->token, varInFunct);
                                        }
                                    }
                                    current = current->nextNode;
                                }
                                free(beforeDot->nextNode->nextNode->token);
                                beforeDot->nextNode->nextNode->token = calloc(2048,sizeof(char));
                                strcpy(beforeDot->nextNode->nextNode->token, newFunctName);
                            } 
                        }
                    }
                    beforeDot = current;
                } while(inFunct != 0 || hitOnce != 1);
                inFunct = 0;
                hitOnce = 0;
                freeList(varInFunct);
            }
        }
        current = current->nextNode;
    }
    free(newFunctName);
    return;
}

int isFunction(item * functionStart) {
    item * current = functionStart->nextNode;
    int inParam = 0;
    while(current != NULL) {
        if (strcmp(current->token, "(") == 0) {
            inParam = 1;
        } else if (strcmp(current->token, ")") == 0) {
            inParam = 0;
        } else if (inParam <= 0){
            if (strcmp(current->token, ";") == 0) {
                return(0);
            }
            else {
                return(1);
            }
        }
        current = current->nextNode;
    }
    return(-1); /* Should Never Happen */
}

char * getStructName(char * structName, item * functVarList) {
    item * current = functVarList->nextNode;
    while(current != NULL) {
        if (strcmp(current->token,"struct") == 0) {
            if (strcmp(current->nextNode->nextNode->token,structName) == 0) {
                return(current->nextNode->token);
            }
        }
        current = current->nextNode;
    }
    return("2750 has claimed another soul"); /* Should Never Happen */
}

char getVarType(char * varName, item * functVarList) {
    item * current = functVarList->nextNode;
    item * beforeName = NULL;
    while(current != NULL) {
        if (strcmp(current->token,"struct") == 0) {
            if (strcmp(current->nextNode->nextNode->token,varName) == 0) {
                return('s');
            } else {
                current = current->nextNode->nextNode->nextNode;
                continue;
            }
        } else {
            if (strcmp(current->token,varName) == 0) {
                return(beforeName->token[0]);
            }
            beforeName = current;
            current = current->nextNode;
        }
    }
    return('\0'); /* Should Never Happen */
}

item * listVarInFunct(item * programList, item * programList2) {
    item * functVarList = initNode();
    item * current = programList->nextNode;
    int inParam = 0;
    while(current != NULL) {
        if (strcmp(current->token,"(") == 0) {
            inParam = 1;
        } else if (strcmp(current->token,")") == 0) {
            inParam = 0;
        } else if (strcmp(current->token,"{") == 0) {
            current = NULL;
            continue;
        } else if (inParam == 1) {
            if (isVarType(current->token) == 1) {
                functVarList = addToBack(functVarList,0,0,0,current->token);
                if (strcmp(current->token,"struct") == 0) {
                    current = current->nextNode;
                    functVarList = addToBack(functVarList,0,0,0,current->token);
                }
                current = current->nextNode;
                while (strcmp(current->token, "*") == 0) {
                    current = current->nextNode;
                }

                if (isPunctuation(current->token[0]) == 0) {
                    functVarList = addToBack(functVarList,0,0,0,current->token);
                }

                while(strcmp(current->nextNode->token, ",") == 0) {
                    current = current->nextNode->nextNode;
                    functVarList = addToBack(functVarList,0,0,0,current->token);
                    if (strcmp(current->token,"struct") == 0) {
                        functVarList = addToBack(functVarList,0,0,0,current->nextNode->token);
                        current = current->nextNode->nextNode;
                    } else {
                        current = current->nextNode;
                    }
                    while (strcmp(current->token, "*") == 0) {
                        current = current->nextNode;
                    }
                    if (isPunctuation(current->token[0]) == 0) {
                        functVarList = addToBack(functVarList,0,0,0,current->token);
                    }
                }
            }
        }
        current = current->nextNode;
    }

    item * structType = NULL;
    item * varType = NULL;
    current = programList;
    int inFunct = 0;
    int hitOnce = 0;
    while (current != NULL) {
        if (strcmp(current->token,"{") == 0) {
            hitOnce = 1;
            inFunct = inFunct + 1;
        } else if (strcmp(current->token,"}") == 0) {
            inFunct = inFunct - 1;
        } else{
            if (isVarType(current->token) == 1  && inFunct > 0) {
                varType = current;
                if (strcmp(current->token,"struct") == 0) {
                    if (strcmp(current->nextNode->nextNode->token,"{") == 0) {
                        current = current->nextNode->nextNode;
                        continue;
                    }
                    current = current->nextNode;
                    structType = current;
                }

                current = current->nextNode;
                while (strcmp(current->token, "*") == 0) {
                    current = current->nextNode;
                }
                if (strcmp(current->nextNode->token, "(") != 0) {
                    if (isPunctuation(current->token[0]) == 0) {
                        functVarList = addToBack(functVarList,0,0,0,varType->token);
                        if (structType != NULL) {
                            functVarList = addToBack(functVarList,0,0,0,structType->token);
                        }
                        functVarList = addToBack(functVarList,0,0,0,current->token);
                    }

                    while(strcmp(current->nextNode->token, ",") == 0) {
                        functVarList = addToBack(functVarList,0,0,0,varType->token);
                        if (strcmp(varType->token,"struct") == 0) {
                            functVarList = addToBack(functVarList,0,0,0,structType->token);
                        }

                        current = current->nextNode->nextNode;
                        if (isPunctuation(current->token[0]) == 0) {
                            functVarList = addToBack(functVarList,0,0,0,current->token);
                        }
                    }
                }
                varType = NULL;
                structType = NULL;
            }
        }
        if (inFunct == 0 && hitOnce == 1) {
            current = NULL;
            continue; 
        }
        current = current->nextNode;
    }

    item * current2 = programList2->nextNode;
    int isGlobal = 0;
    varType = NULL;
    structType = NULL;
    inParam = 0;

    while(current2 != NULL) {
        if (strcmp(current2->token,"{") == 0) {
            isGlobal ++;
        }

        if (strcmp(current2->token,"}") == 0) {
            isGlobal = isGlobal - 1;
        }

        if (strcmp(current2->token,")") == 0) {
            inParam = 0;
        }

        if (isVarType(current2->token) == 1 && isGlobal == 0 && inParam == 0) {
            varType = current2;
            structType = NULL;
            if (strcmp(current2->token,"struct") == 0) {
                if (strcmp(current2->nextNode->nextNode->token,"{") == 0) {
                    current2 = current2->nextNode->nextNode;
                    continue;
                }
                current2 = current2->nextNode;
                structType = current2;
            }

            current2 = current2->nextNode;
            while (strcmp(current2->token, "*") == 0) {
                current2 = current2->nextNode;
            }
            if (strcmp(current2->nextNode->token, "(") != 0) {
                if (isPunctuation(current2->token[0]) == 0) {
                    functVarList = addToBack(functVarList,0,0,0,varType->token);
                    if (structType != NULL) {
                        functVarList = addToBack(functVarList,0,0,0,structType->token);
                    }
                    functVarList = addToBack(functVarList,0,0,0,current2->token);
                }

                while(strcmp(current2->nextNode->token, ",") == 0) {
                        functVarList = addToBack(functVarList,0,0,0,varType->token);
                        if (strcmp(varType->token,"struct") == 0) {
                            functVarList = addToBack(functVarList,0,0,0,structType->token);
                        }

                        current2 = current2->nextNode->nextNode;
                        if (isPunctuation(current2->token[0]) == 0) {
                            functVarList = addToBack(functVarList,0,0,0,current2->token);
                        } 
                }
                varType = NULL;
                structType = NULL;
            } else {
                inParam = 1;
            }
        }

        current2 = current2->nextNode;
    }
    return(functVarList);
}
/*  */
void replaceVarInClassFunct(item * programList) {
    item * current = programList->nextNode;
    item * newNode = NULL;
    while (current != NULL) {
        if (current->nextNode != NULL) {
            if (current->nextNode->isClassVar == 1) {
                newNode = makeConstructorNode("sPtrNM", current->nextNode->numNewLines, current->nextNode->numWhiteSpace);
                current = insertNext(current,newNode);
                current = current->nextNode;
                newNode = makeConstructorNode("-", 0, 0);
                current = insertNext(current,newNode);
                current = current->nextNode;
                newNode = makeConstructorNode(">", 0, 0);
                current = insertNext(current,newNode);
                current = current->nextNode;
                if (current->nextNode != NULL) {
                    current->nextNode->numWhiteSpace = 0;
                    current->nextNode->numTabs = 0;
                    current->nextNode->numNewLines = 0;
                }
            }
        }
        current = current->nextNode;
    }
}


/* List Functions */

/* Creates node and sets values to 0. */
item * initNode(){
    item * newNode = malloc(sizeof(item)*1);
    newNode->token = NULL;
    newNode->isFunction = 0;
    newNode->numWhiteSpace = 0;
    newNode->numTabs = 0;
    newNode->numNewLines = 0;
    newNode->isClassVar = 0;
    newNode->nextNode = NULL;
    return(newNode);
}

/* Adds the information to the back of the list. */
item * addToBack(item * theList, int numWhiteSpace, int numTabs, int numNewLines, char * toBeAdded) {
    item * newNode = NULL;
    item * curr = NULL;
    
    if (theList == NULL || toBeAdded == NULL) {
        return(NULL);
    } else {
        newNode = initNode();
        newNode->token = malloc(sizeof(char) * (strlen(toBeAdded)+1));
        strcpy(newNode->token, toBeAdded);
        newNode->numWhiteSpace = numWhiteSpace;
        newNode->numTabs = numTabs;
        newNode->numNewLines = numNewLines;
        newNode->nextNode = NULL;
        curr = theList;
        while (curr->nextNode != NULL) {
            curr= curr->nextNode;
        }
        curr->nextNode = newNode;
    }
    return(theList);
}

/* Inserts the next item into the list. */
item * insertNext(item * theList, item * newNode) {
    item * temp = NULL;
    temp = theList->nextNode;
    theList->nextNode = newNode;
    newNode->nextNode = temp;
    return(theList);
}

/* Checks to see if the item given is in the list. */
int isInList(item * theList, char * token) {
    item * current = theList->nextNode;
    while(current != NULL){
        if (strcmp(current->token, token) == 0) {
            return(1);
        }
        current = current->nextNode;
    }
    return(0);
}

/* Prints the lists contents. */
void printList (item * theList) {
    item * currListPos = theList->nextNode;
    
    if (theList == NULL) {
        printf("Error. NULL list. Returning.\n");
        return;
    } else {
        while (currListPos != NULL) {
            printf("~~%s~~\n",currListPos->token);
            currListPos = currListPos->nextNode;
        }
    }    
    return;
}

/* Frees the list and its contents. */
void freeList (item * theList) {
    item * currListPos = theList->nextNode;
    
    if (theList == NULL) {
        printf("Error. NULL list. Returning.\n");
        return;
    } else {
        while (currListPos != NULL) {
            item * temp = currListPos;
            free(temp->token);
            currListPos = currListPos->nextNode;
            free(temp);
        }
    }
    if (theList->token != NULL) {
        free(theList->token);
    }
    free(theList);
    return;
}
