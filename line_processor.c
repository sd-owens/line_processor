#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define SIZE 1000

// Buffers, Shared Thread Resources
char t1_buffer[SIZE];
char t2_buffer[SIZE];
char t3_buffer[SIZE];

// MULTI-THREADED VERSION OF LINE PROCESSOR PROGRAM

// Gets input from stdin stream
int getInputLine(char *buffer){

    fgets(buffer, SIZE, stdin);
    
    return 0;
}

// Parses input to replace line seperators '\n' with ' ' spaces (blanks)
int parseLines(char *output, char *input){
    
    strcat(output, input);

    // Clear the input buffer for reuse
    memset(input, '\0', strlen(input));

    int i = 0;
    while (output[i] != '\0'){

        if(output[i] == '\n'){
            output[i] = ' '; 
        }
        i++;
    }
    return 0;
}

// Parses lines to replace instance of '++' with '^' character
int parseChars(char *output, char *input){

    strcat(output, input);

    // Clear the input buffer for reuse
    memset(input, '\0', strlen(input));

    int i = 0;
    while (output[i] != '\0'){

        if(output[i] == '+' && output[i+1] == '+'){

            output[i] = '^';
            // Copy all chars left one position to fill gap created.
            strcpy(output + i + 1, output + i + 2);
        }
        i++;
    }
    return 0;
}
// Writes parsed lines to stdout steam
int writeOutput(char *buffer){

    // Print 80 char limit line to terminal
    while (strlen(buffer) > 79) {

        int i = 0;
        while (i < 79){

            fputc(buffer[i], stdout);
            i++;
        }
        fputc('\n', stdout);

        // Update buffer by overwriting written characters
        strcpy(buffer, buffer + 79);
    }
    return 0;
}

int main(int argc, char *argv[]){

    bool cont = true;

    while(cont){

        getInputLine(t1_buffer);
        if(strcmp(t1_buffer, "DONE\n") == 0){
            cont = false;
            continue;
        }
        parseLines(t2_buffer, t1_buffer);
        parseChars(t3_buffer, t2_buffer);
        writeOutput(t3_buffer);
    }
    return 0;
}