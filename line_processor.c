#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

// Sized for 10 input lines of max 1000 chars
#define SIZE 10000

// MULTI-THREADED VERSION OF LINE PROCESSOR PROGRAM

// Buffers, Shared Thread Resources
char t1_buffer[SIZE];
char t2_buffer[SIZE];
char t3_buffer[SIZE];

// Number of items in t1 buffer, shared resource
int t1_count = 0;
// Number of items in t2 buffer, shared resource
int t2_count = 0;
// Number of items in t3 buffer, shared resource
int t3_count = 0;


// Initialize the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variables
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

// Gets input from stdin stream
int getInputLine(char *buffer){

    fgets(buffer, SIZE, stdin);
    
    return 0;
}

void *input_producer(void *args){

    pthread_mutex_lock(&mutex);
    while(t1_count != 0){

        // t1_buffer is still full
        pthread_cond_signal(&full);
    }
    // Signal to consumer that buffer is not longer empty
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);

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

void *input_consumer(void *args){

    pthread_mutex_lock(&mutex);
    while(t1_count != 0){

        // t1_buffer is still full
        pthread_cond_signal(&full);
    }
    parseLines(t2_buffer, t1_buffer);
    // Signal to consumer that buffer is not longer empty
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);

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

    pthread_t input, line_seperator;
    //plus_sign, output;

    pthread_create(&input, NULL, input_producer, NULL);
    pthread_create(&line_seperator, NULL, input_consumer, NULL);
    pthread_join(input, NULL);
    pthread_join(line_seperator, NULL);

    // while(cont){

    //     getInputLine(t1_buffer);
    //     if(strcmp(t1_buffer, "DONE\n") == 0){
    //         cont = false;
    //         continue;
    //     }
    //     parseLines(t2_buffer, t1_buffer);
    //     parseChars(t3_buffer, t2_buffer);
    //     writeOutput(t3_buffer);
    // }
    return 0;
}