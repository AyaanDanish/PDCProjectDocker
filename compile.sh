#!/bin/bash

clear;
gcc main.c -o main;

gcc ./Djikstra/djikstra_serial.c -o ./Djikstra/djikstra_serial -fopenmp; 
gcc ./Djikstra/djikstra_parallel.c -o ./Djikstra/djikstra_parallel -fopenmp;

gcc ./Sudoku/sudoku_serial.c -o ./Sudoku/sudoku_serial -fopenmp ;
gcc ./Sudoku/sudoku_parallel.c -o ./Sudoku/sudoku_parallel -fopenmp;

gcc ./Vowels/vowel_consonant_serial.c -o ./Vowels/vowel_consonant_serial -fopenmp;
gcc ./Vowels/vowel_consonant_parallel.c -o ./Vowels/vowel_consonant_parallel -fopenmp;

./main