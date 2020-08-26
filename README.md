# Multi-threaded producer-consumer pipeline written in c and tlpi

C PROGRAM
Compiled with GNU Compiler GCC

Must used flag -std=c99 to compile otherwise syntax of for loops is invalid.
(Prior to c99 for loop variables must be declared outside(before) for loops not in the declaration)

Example usage:

Line Processor

gcc -std=c99 -g -pthread -lm -Wall -pedantic line_processor.c -o line_processor

./line_processor
./line_processor < <inputfile>  
(e.g. ./line_processor < input1.txt)
