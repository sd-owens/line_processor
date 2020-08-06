#!/bin/bash
gcc -std=c99 -Wall -pedantic line_processor_st.c -o line_processor_st
gcc -std=c99 -Wall -pedantic line_processor.c -o line_processor
#./line_processor