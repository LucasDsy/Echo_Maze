#include "../headers/maze.h"

int **get_maze(unsigned long int dimension) {
    int **maze = malloc(sizeof(int*)*dimension);
    int i, j;
    char *line = NULL;
    size_t length = 0;

    FILE *file = fopen(FILEPATH, "r");

    for(i=0;i<(int) dimension;i++) {
        getline(&line, &length, file);
        maze[i] = malloc(sizeof(int*)*dimension);
        for(j=0;j<(int) dimension;j++) {
            maze[i][j] = (int) line[j] - 48; // ASCII to int
        }
    }

    if(line)
        free(line);    

    fclose(file);
    
    return maze;
}

void display_maze(int DIM, int **maze) {
    int i, j;
    for(i=0;i<DIM;i++) {
        for(j=0;j<DIM;j++) {
            printf("%d", *(*(maze+i)+j));
        }
        printf("\n");
    }
}