#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>

// Sized for 10 input lines of max 1000 chars
#define LINE        1000
#define SIZE        10000

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

// Gets input from stdin stream and outputs to the shared t1_buffer
void *input(void *args){

    bool END_MARKER = false;

    char data[LINE];
    //memset(data, '\0', sizeof(data));

    while(!END_MARKER){
       
        fgets(data, LINE, stdin);
        //fflush(stdin);  // <-- THIS TOOK OVER 30 HOURS TO FIGURE OUT!!!!

        // If DONE is received, flag the END_MARKER as true
        if(strcmp(data, "DONE\n") == 0){
            END_MARKER = true;
        } 

        if(strlen(data) > 0) {
            // Lock the mutex before writing to t1_buffer
            pthread_mutex_lock(&mutex1);
            strcat(t1_buffer, data);

            // Signal to consumer that buffer is no longer empty
            pthread_cond_signal(&full);
            // Unlock shared buffer (mutex1)
            pthread_mutex_unlock(&mutex1);
        }
    }
    return NULL;
}

// Parses input to replace line seperators '\n' with ' ' spaces (blanks)
int parseLines(char *output){
    
    int i = 0;
    while (output[i] != '\0'){

        if(output[i] == '\n'){
            output[i] = ' '; 
        }
        i++;
    }
    return 0;
}
// Line Seperator Thread to execute parseLine function
void *line_seperator(void *args){

    bool END_MARKER = false;

    char data[LINE];

    while(!END_MARKER){

        // Lock the input buffer before checking contents
        pthread_mutex_lock(&mutex1);
        while(strlen(t1_buffer) == 0){
            // Buffer is empty. Wait for signal from
            // input thread that buffer has data
            pthread_cond_wait(&full, &mutex1);
        }

        strcpy(data, t1_buffer);
        // Signal to producer that buffer been emptied and
        // unlocked shared buffer (mutex1)
        memset(t1_buffer, '\0', strlen(t1_buffer));
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex1);

        // If DONE is received, flag the END_MARKER as seen
        // and don't process the newlines in the buffer.
        if(strstr(data, "DONE\n") != NULL){
            END_MARKER = true;
        } 
        parseLines(data);
        
        // Lock output buffer mutex
        pthread_mutex_lock(&mutex2);
        // Write formatted string to output buffer
        strcat(t2_buffer, data);
        //printf("T2 BUFFER\n%s\n", t2_buffer);

        // Signal to consumer that buffer been filled
        // Unlock shared buffer (mutex2)
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

// Parses lines to replace instance of '++' with '^' character
int parseChars(char *output){

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

    bool END_MARKER = false;

    char data[LINE];

    while(!END_MARKER){

        // Lock the mutex before checking if buffer is empty
        pthread_mutex_lock(&mutex2);
        while(strlen(t2_buffer) == 0){

            // t2_buffer is empty.   Wait for producer to signal
            // that input buffer has been filled.
            pthread_cond_wait(&full, &mutex2);
        }
       
        strcpy(data, t2_buffer);
        memset(t2_buffer, '\0', strlen(t2_buffer));
        // Signal to producer that buffer been emptied and
        // unlocked shared buffer (mutex2)
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex2);

        if(strstr(data, "DONE ") != NULL){
            END_MARKER = true;
        } 

        parseChars(data);
        // Lock output buffer mutex
        pthread_mutex_lock(&mutex3);
        // Write formatted formatted string to output buffer
        strcat(t3_buffer, data);

        // Signal to consumer that buffer been filled
        // Unlock shared buffer (mutex3)
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex3);
    }
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

    bool END_MARKER = false;

    char data[LINE];

    while(!END_MARKER){

        pthread_mutex_lock(&mutex3);
        while(strlen(t3_buffer) == 0){
            // Buffer is empty. Wait for signal from
            // plus_sign thread that buffer has been filled
            pthread_cond_wait(&full, &mutex3);
        }

        strcat(data, t3_buffer);
        // Signal to producer that buffer has been emptied.
        memset(t3_buffer, '\0', strlen(t3_buffer));
        pthread_cond_signal(&empty);
        // Unlock the mutex
        pthread_mutex_unlock(&mutex3);

        if(strstr(data, "DONE ") != NULL){
            END_MARKER = true;
            
        }
        writeOutput(data);
    }
    return NULL;
}

int main(int argc, char *argv[]){

    pthread_t t1, t2, t3, t4;

    // Spawn individual threads for pipeline
    pthread_create(&t1, NULL, input, NULL);
    pthread_create(&t2, NULL, line_seperator, NULL);
    pthread_create(&t3, NULL, plus_sign, NULL);
    pthread_create(&t4, NULL, output, NULL);
  
    // Rejoin spawned threads with Main
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    exit(EXIT_SUCCESS);
}