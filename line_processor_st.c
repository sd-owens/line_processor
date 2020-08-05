#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFERSIZE 1000
#define OUTPUTSIZE 80

// Gets input from stdin stream
int getInput(int argc){

    char* input = calloc(1,1), buffer[BUFFERSIZE];

    
    // printf("Enter a message: \n");

    while(fgets(buffer, BUFFERSIZE, stdin)){

        input = realloc(input, strlen(buffer));
        strcpy(input, buffer);
        input[strlen(input) - 1] = '\0';  // replace '\n' with '\0'
        if(strcmp(input, "DONE") == 0) {break;}
        memset(buffer, '\0', sizeof(buffer));
    }
    free(input);
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

    fprintf(stdout, "args = %d\n", argc);
    getInput(argc);
    parseLines();
    parseChars();
    writeOutput("Goodbye\n");

    return 0;
}