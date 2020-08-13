#!/bin/bash
gcc -std=c99 -g -pthread -lm -Wall -pedantic line_processor.c -o line_processor
#./line_processor