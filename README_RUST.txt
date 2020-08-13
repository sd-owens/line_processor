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