#include <stdlib.h>
#include <stdio.h>

// Libs
#include "headers/sdl.h"
#include "headers/openal.h"
#include "headers/maze.h"

#define DIM 15

int main() {
    
    int **myMaze = get_maze(DIM);
    display_maze(DIM, myMaze);

    //OpenAL
    init_openal();

    // SDL
    
    init_sdl(DIM);
    init_grid(DIM, myMaze);
    
    t_player player;
	player.x = 0;
	player.y = 0;

    t_exit exit;
	exit.x = 14;
	exit.y = 13;

    animate_player(DIM, myMaze, player);
    runner(DIM, myMaze, player, exit);

    //exit_openal();
    
    return 0;
}