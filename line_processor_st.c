#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFERSIZE 1000
#define OUTPUTSIZE 80

// Gets input from stdin stream
int getInputLine(char *buffer){

    fgets(buffer, BUFFERSIZE, stdin);
    
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

    // int i = 0;
    // while (output[i] != '\0'){

    //     if(output[i] == '+' && output[i+1] == '+'){



    //     }
    //     i++;
    // }



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

        //fprintf(stdout, "%d\n", i + 1);
        
        // Update buffer by overwriting written characters
        strcpy(buffer, buffer + 79);
        //fprintf(stdout, "%s\n", buffer);
    }

    return 0;
}


int main(int argc, char *argv[]){

    char t1_buffer[BUFFERSIZE];
    char t2_buffer[BUFFERSIZE];
    char t3_buffer[BUFFERSIZE];

    bool cont = true;
    int count = 1;
    
    //printf("Enter a message: \n");

    while(cont){

        getInputLine(t1_buffer);
        //TODO "DONE " has an extra space remaining
        if(strcmp(t1_buffer, "DONE\n") == 0){
            cont = false;
        }
        parseLines(t2_buffer, t1_buffer);
        parseChars(t3_buffer, t2_buffer);
        writeOutput(t3_buffer);
        count++;
    }
    
    return 0;
}