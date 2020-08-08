#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>

// Sized for 10 input lines of max 1000 chars
#define SIZE        10000

// Special marker to indicate end of input data
bool END_MARKER = false;

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

    while(!END_MARKER){

        // Lock the mutex before writing to input_buffer
        pthread_mutex_lock(&mutex1);
    
        read_data = getInputLine(t1_buffer);

        // If DONE is received, flag the END_MARKER as seen.
        if(strcmp(read_data, "DONE\n") == 0){

            END_MARKER = true;
        }

        // Signal to consumer that buffer is no longer empty and
        // unlock shared buffer (mutex1)
        pthread_cond_signal(&full);
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
    // Lock output buffer mutex
    pthread_mutex_lock(&mutex2);
    parseLines(t2_buffer, t1_buffer);
    // Signal to producer that buffer been emptied and
    // unlocked shared buffer (mutex1)
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex1);
    // Signal to consumer that buffer been filled and
    // unlocked shared buffer (mutex2)
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex2);
    
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
            
            int j = i + 1;
            
            // Fill the gap created by adding '^' in place of '++'
            while(output[j] != '\0'){

                output[j] = output[j+1];
                j++;
            }
        }
        i++;
    }
    return 0;
}
void *plus_sign(void *args){

    // Lock the mutex before checking if buffer is empty
    pthread_mutex_lock(&mutex2);
    while(strlen(t2_buffer) == 0){

        // t2_buffer is empty.   Wait for producer to signal
        // that buffer has been filled.
        pthread_cond_wait(&full, &mutex2);
    }
    // Lock output buffer mutex
    pthread_mutex_lock(&mutex3);
    parseChars(t3_buffer, t2_buffer);
    // Signal to producer that buffer been emptied and
    // unlocked shared buffer (mutex2)
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex2);
    // Signal to consumer that buffer been filled and
    // unlocked shared buffer (mutex3)
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex3);
    
    return NULL;
}

// Writes parsed lines to stdout steam
int writeOutput(char *buffer){

    // Print 80 char limit line to terminal
    while (strlen(buffer) >= 80) {

        int i = 0;
        while (i < 80){

            fputc(buffer[i], stdout);
            fflush(stdout);
            i++;
        }
        fputc('\n', stdout);
        fflush(stdout);

        // Update buffer by overwriting output chars
        strcpy(buffer, buffer + 80);
    }
    return 0;
}

void *output(void *args){

    while (!END_MARKER){

    pthread_mutex_lock(&mutex3);
    while(strlen(t3_buffer) == 0){
        // Buffer is empty. Wait for signal from
        // plus_sign thread that buffer has been filled
        pthread_cond_wait(&full, &mutex3);
    }

    writeOutput(t3_buffer);
    // Signal to producer that buffer size has been emptied.
    pthread_cond_signal(&empty);
    // Unlock the mutex
    pthread_mutex_unlock(&mutex3);
    }

    return NULL;
}

int main(int argc, char *argv[]){

    pthread_t t1, t2, t3, t4;

    pthread_create(&t1, NULL, input, NULL);
    pthread_create(&t2, NULL, line_seperator, NULL);
    pthread_create(&t3, NULL, plus_sign, NULL);
    pthread_create(&t4, NULL, output, NULL);
  
    //printf("joining thread t1...\n");
    pthread_join(t1, NULL);
    //printf("joining thread t2...\n");
    pthread_join(t2, NULL);
    //printf("joining thread t3...\n");
    pthread_join(t3, NULL);
    //printf("joining thread t4...\n");
    pthread_join(t4, NULL);

    return 0;
}