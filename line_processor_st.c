#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFERSIZE 1000
#define OUTPUTSIZE 80

// Gets input from stdin stream
int getInputLine(char *line, char * buffer){

    line = realloc(line, strlen(buffer));
    strcpy(line, buffer);
    line[strlen(line) - 1] = '\0';  // replace '\n' with '\0'
    
    return 0;
}


// Parses input to replace line seperators '\n' with ' ' spaces (blanks)
int parseLines(){
    return 0;
}

// Parses lines to replace instance of '++' with '^' character
int parseChars(){
    return 0;
}
// Writes parsed lines to stdout steam
int writeOutput(char *line){


    fprintf(stdout, "%s", line);

    return 0;
}


int main(int argc, char *argv[]){

    char* line = calloc(1,1), buffer[BUFFERSIZE];
    
    //printf("Enter a message: \n");

    while(fgets(buffer, BUFFERSIZE, stdin)){

        getInputLine(line, buffer);
        if(strcmp(line, "DONE") == 0){
            break;
        }
        parseLines();
        parseChars();
        writeOutput(line);
        memset(buffer, '\0', sizeof(buffer));
    }

    free(line);
    
    return 0;
}