#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#define FILEPATH "./utils/maze.txt"

int **get_maze(unsigned long int dimension);
void display_maze(int DIM, int **maze);