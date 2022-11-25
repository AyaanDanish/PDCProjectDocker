#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main() {

  int choice;

  printf("Enter choice: \n");
  printf("1. Sudoku Solver Algorithm\n"
         "2. Djikstra's Algorithm\n\nChoice: ");

  scanf(" %d", &choice);

  switch (choice) {
  case 1:
    if (fork())
      execl("./Sudoku/sudoku_serial", "./Sudoku/sudoku_serial", NULL);
    else
      execl("./Sudoku/sudoku_parallel", "./Sudoku/sudoku_parallel", NULL);
    break;

  case 2:
    if (fork())
      execl("./Djikstra/djikstra_serial", "./Djikstra/djikstra_serial", NULL);
    else
      execl("./Djikstra/djikstra_parallel", "./Djikstra/djikstra_parallel",
            NULL);
    break;
  default:
    printf("Invalid Entry!\n");
  }
}