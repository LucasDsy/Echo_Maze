#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#define FILEPATH "./utils/maze.txt"
#define SIZE 15


int** readMaze(unsigned long int dimension) {
    int** maze = malloc(sizeof(int*) * dimension);
    char* line = NULL;
    size_t length = 0;

    FILE* file = fopen(FILEPATH, "r");

    for (int i = 0; i < (int) dimension; i++) {
        getline(&line, &length, file);
        maze[i] = malloc(sizeof(int*)*dimension);
        
        for (int j = 0; j < (int) dimension; j++) {
            maze[i][j] = (int) line[j] - 48; // ASCII to int
        }
    }

    if (line)
        free(line);

    fclose(file);
    
    return maze;
}


void displayMaze(int** maze) {
    for (int i = 0; i < SIZE; i++) {

        for (int j = 0; j < SIZE; j++)
            printf("%d", *(*(maze+i)+j));

        printf("\n");
    }
}


distances distancesToWall(int** maze, t_position player) {
    distances d;
    int i, j;

    // Distance est
    i = player.x;
    j = player.y;
    
    while (i < SIZE && maze[i][j] != 0)
        i++;

    d.est = i - player.x;


    // Distance ouest
    i = player.x;
    j = player.y;
    
    while (i > 0 && maze[i][j] != 0)
        i--;

    d.ouest = player.x - i;


    // Distance nord
    i = player.x;
    j = player.y;
    
    while (j > 0 && maze[i][j] != 0)
        j--;

    d.nord = player.y - j;


    // Distance sud
    i = player.x;
    j = player.y;
    
    while (j < SIZE && maze[i][j] != 0)
        j++;

    d.sud = j - player.y;

    return d;
}