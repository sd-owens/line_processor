#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>

// Sized for 10 input lines of max 1000 chars
#define SIZE 10000

// MULTI-THREADED VERSION OF LINE PROCESSOR PROGRAM

// Buffers, Shared Thread Resources
char t1_buffer[SIZE];
char t2_buffer[SIZE];
char t3_buffer[SIZE];

// Initialize the mutexes
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variables
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

// Gets input from stdin stream
char * getInputLine(char *buffer){

    int offset = strlen(buffer);

    char *c = fgets(buffer + offset, SIZE, stdin);
    
    return c;
}

void *input(void *args){

    char *read_data;

    while (strcmp(read_data, "DONE\n") != 0){

        // // Lock the mutex before writing to input_buffer
        pthread_mutex_lock(&mutex1);
        // while(strlen(t1_buffer) != 0){

        //     // t1_buffer has data.  Wait for consumer to signal
        //     // that buffer has been emptied.
        //     pthread_cond_wait(&empty, &mutex1);
        // }
        read_data = getInputLine(t1_buffer);
        // Signal to consumer that buffer is no longer empty
        pthread_cond_signal(&full);
        // Unlock the mutex
        pthread_mutex_unlock(&mutex1);
    }

    return NULL;
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

void *line_seperator(void *args){

    pthread_mutex_lock(&mutex1);
    while(strlen(t1_buffer) == 0){
        // Buffer is empty. Wait for signal from
        // input thread that buffer has data
        pthread_cond_wait(&full, &mutex1);
    }
    parseLines(t2_buffer, t1_buffer);
    // Signal to producer that buffer has been emptied
    pthread_cond_signal(&empty);
    // Unlock the mutex
    pthread_mutex_unlock(&mutex1);

    // // Lock the mutex before checking if buffer is empty
    // pthread_mutex_lock(&mutex2);
    // while(1){

    //     // t2_buffer is full,  wait for signal from 
    //     // plus_sign thread that buffer has been emptied.
    //     pthread_cond_wait(&empty, &mutex2);
    // }
   
    // // Signal to consumer that buffer is no longer empty
    // pthread_cond_signal(&full);
    // // Unlock the mutex
    // pthread_mutex_unlock(&mutex2);

    return NULL;
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
void *plus_sign(void *args){

    // CONSUMER FOR T2_BUFFER
    // Lock the mutex before checking if buffer is empty
    pthread_mutex_lock(&mutex2);
    while(strlen(t2_buffer) == 0){

        // t2_buffer is empty.   Wait for producer to signal
        // that buffer has been filled.
        pthread_cond_wait(&full, &mutex2);
    }
    parseChars(t3_buffer, t2_buffer);
    // Signal to producer that buffer been emptied
    pthread_cond_signal(&empty);
    // Unlock the mutex
    pthread_mutex_unlock(&mutex2);

    // // PRODUCER FOR T3_BUFFER
    // // Lock the mutex before checking if buffer is empty
    // pthread_mutex_lock(&mutex3);
    // while(strlen(t3_buffer) >= 79){

    //     // t3_buffer is full,  wait for signal from 
    //     // output thread that buffer has been emptied.
    //     pthread_cond_wait(&empty, &mutex3);
    // }
    
    // // Signal to consumer that buffer is no longer empty
    // pthread_cond_signal(&full);
    // // Unlock the mutex
    // pthread_mutex_unlock(&mutex3);

    return NULL;
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

void *output(void *args){

    pthread_mutex_lock(&mutex3);
    while(strlen(t3_buffer) < 79){
        // Buffer is effectively empty. Wait for signal from
        // plus_sign thread that buffer has > 79 chars in buffer
        pthread_cond_wait(&full, &mutex3);
    }
    writeOutput(t3_buffer);
    // Signal to producer that buffer size is less than 79 chars
    pthread_cond_signal(&empty);
    // Unlock the mutex
    pthread_mutex_unlock(&mutex3);

    return NULL;
}

int main(int argc, char *argv[]){

    //bool cont = true;
    int s;
    pthread_t t1, t2, t3, t4;

    s = pthread_create(&t1, NULL, input, NULL);
    if(s != 0)
        fprintf(stderr, "pthread_create");
    s = pthread_create(&t2, NULL, line_seperator, NULL);
    if(s != 0)
        fprintf(stderr, "pthread_create");
    s = pthread_create(&t3, NULL, plus_sign, NULL);
    if(s != 0)
        fprintf(stderr, "pthread_create");
    s = pthread_create(&t4, NULL, output, NULL);
    if(s != 0)
        fprintf(stderr, "pthread_create");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

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