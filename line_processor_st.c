#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFERSIZE 1000
#define OUTPUTSIZE 80

// Gets input from stdin stream
int getInputLine(char *buffer){

    // Clear the buffer for reuse
    memset(buffer, '\0', strlen(buffer));
    fgets(buffer, BUFFERSIZE, stdin);
    
    return 0;
}


// Parses input to replace line seperators '\n' with ' ' spaces (blanks)
int parseLines(char *output, char *input){
    
    strcat(output, input);

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
int parseChars(char *t2_buffer, char *t3_buffer){

    int i = 0;
    while (t2_buffer[i] != '\0'){

        if(t2_buffer[i] == '+' && t2_buffer[i+1] == '+'){



        }
        i++;
    }



    return 0;
}
// Writes parsed lines to stdout steam
int writeOutput(char *buffer){

    // Print 80 char limit line to terminal
    if(strlen(buffer) > 79) {

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
    
    //printf("Enter a message: \n");

    while(cont){

        getInputLine(t1_buffer);
        parseLines(t2_buffer, t1_buffer);
        parseChars(t2_buffer, t3_buffer);

        //TODO "DONE " has an extra space remaining
        if(strcmp(t2_buffer, "DONE ") == 0){
            cont = false;
            break;
        }

        writeOutput(t2_buffer);
        //memset(buffer, '\0', sizeof(buffer));
    }
    
    return 0;
}