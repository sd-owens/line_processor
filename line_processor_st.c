#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFERSIZE 1000
#define OUTPUTSIZE 80

// Gets input from stdin stream
int getInputLine(char *t1_buffer){

    fgets(t1_buffer, BUFFERSIZE, stdin);

    //line[strlen(line) - 1] = ' ';  // replace '\n' with ' '
    
    return 0;
}


// Parses input to replace line seperators '\n' with ' ' spaces (blanks)
int parseLines(char *t1_buffer, char *t2_buffer){
    
    strcpy(t2_buffer, t1_buffer);

    int i = 0;
    while (t2_buffer[i] != '\0'){

        if(t2_buffer[i] == '\n'){
            t2_buffer[i] = ' '; 
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
int writeOutput(char *line){

    if(strlen(line) > 79) {

        int i = 0;
        while (i < 79){

            fputc(line[i], stdout);
            i++;
        }
        fputc('\n', stdout);
        i++;

        //fprintf(stdout, "%d\n", i);
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
        parseLines(t1_buffer, t2_buffer);
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