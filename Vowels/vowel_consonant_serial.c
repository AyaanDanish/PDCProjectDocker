#include <ctype.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long len;

char *read_file(char *filename) {
  FILE *file;
  file = fopen(filename, "r");

  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  len = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *string = malloc(sizeof(char) * (len + 1));

  char c;
  int i = 0;

  while ((c = fgetc(file)) != EOF) {
    string[i] = c;
    i++;
  }

  string[i] = '\0';

  fclose(file);

  return string;
}

int main() {

  // char *line;
  double start, end;
  int vowels, consonant, digit, space;

  // initialize all variables to 0
  vowels = consonant = digit = space = 0;

  // get full line of string input
  // printf("Enter a line of string: ");
  // fgets(line, sizeof(line), stdin);

  char *filename = "./Vowels/Dataset.txt";
  char *line = read_file(filename);

  if (line == NULL) {
    printf("Error reading file\n");
    return 1;
  }

  // free(line);

  start = omp_get_wtime();
  // loop through each character of the string
  for (int i = 0; i < len; ++i) {

    // convert character to lowercase
    line[i] = tolower(line[i]);

    // check if the character is a vowel
    if (line[i] == 'a' || line[i] == 'e' || line[i] == 'i' || line[i] == 'o' ||
        line[i] == 'u') {

      // increment value of vowels by 1
      ++vowels;
    }

    // if it is not a vowel and if it is an alphabet, it is a consonant
    else if ((line[i] >= 'a' && line[i] <= 'z')) {
      ++consonant;
    }

    // check if the character is a digit
    else if (line[i] >= '0' && line[i] <= '9') {
      ++digit;
    }

    // check if the character is an empty space
    else if (line[i] == ' ') {
      ++space;
    }
  }

  end = omp_get_wtime();

  printf("\nSERIAL\nVowels: %d", vowels);
  printf("\nConsonants: %d", consonant);
  printf("\nDigits: %d", digit);
  printf("\nWhite spaces: %d\n", space);

  printf("Time Taken (SERIAL): %f s\n\n", (end - start));
  return 0;
}
