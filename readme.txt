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


RUST PROGRAM
Compiled with rustc 2018

Example usage:

rustc main.rs
./main <num_partitions> <elements>
(e.g. ./main 8 16)

Notes: From Piazza post @209 I chose strategy #1  One partition is smaller than the others.  the
examples provided work, but if the math works out that num_partitions is less than the minimum 
partition size causing the last bin to be less than 0, then the program panics (terminates).

(e.g. ./main 5 6) causes error since this would make the last bin -2).

from Professor Chaudry:  "If you have chosen this strategy, you can assume that the values of 
num_partitions and num_elements will be such that the smallest partition will still have at least one 
element." 

Therefore, ./main 5 9 would be one possible solution {2, 2, 2, 2, 1}, also ./main 1 3 {2, 1};
