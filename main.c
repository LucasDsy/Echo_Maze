#include "headers/sdl.h"
#include "headers/openal.h"
#include "headers/maze.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)<(b))?(b):(a))

// OpenAL global variables
ALuint source;
ALuint buffer;
EFXEAXREVERBPROPERTIES reverb;


void runner(int** maze, t_position player, t_position exit) {
    
    static SDL_Event event;
    static const Uint8 *currentKeyStates = NULL;
	currentKeyStates = SDL_GetKeyboardState(NULL);

    // FIXME: touche quitter Q
    while (!(event.type == SDL_QUIT || (player.x == exit.x && player.y == exit.y))) {
		SDL_WaitEvent(&event);
		SDL_PumpEvents();
        
		if (currentKeyStates[SDL_SCANCODE_UP] && maze[player.x][player.y - 1]) {
            player.y = MAX(0, player.y - 1);
            player.d = NORD;
        }

		else if (currentKeyStates[SDL_SCANCODE_DOWN] && maze[player.x][player.y + 1]) {
			player.y = MIN(SIZE - 1, player.y + 1);
            player.d = SUD;
        }

		else if (currentKeyStates[SDL_SCANCODE_LEFT] && maze[player.x - 1][player.y]) {
			player.x = MAX(0, player.x - 1);
            player.d = OUEST;
        }

		else if (currentKeyStates[SDL_SCANCODE_RIGHT] && maze[player.x + 1][player.y]) {
			player.x = MIN(SIZE - 1, player.x + 1);
            player.d = EST;
        
        }
		
        // On bouge le joueur à sa nouvelle position
        movePlayer(maze, SIZE, player);

        // On rectifie l'orientation du joueur
        //setOrientation((int)player.d);

        // On va chercher la distance entre le joueur et chaque mur
        distances d = distancesToWall(maze, player);

        // On modifie le reverb en conséquence
        reverb.flReflectionsPan[0] = (float) (d.sud-d.nord)/SIZE;
        reverb.flReflectionsPan[2] = (float) (d.est-d.ouest)/SIZE;

        printf("Coordonnées joueur : x:%d y:%d d:%d\n", player.x, player.y, player.d);
        printf("reflexionPan Sud-Nord:%lf\n", reverb.flReflectionsPan[0]);
        printf("reflexionPan Est-Ouest:%lf\n\n", reverb.flReflectionsPan[2]);
        
        // On joue le son avec le reverb approprié
        playSourceWithReverb(source, reverb);
	}
}


int main() {

    // Initialisation labyrinthe
    int** maze = readMaze();

    // Initialisation OpenAL
    initOpenAL(&buffer, &source, &reverb);

    // Initialisation SDL
    initSDL(SIZE);
    initGrid(maze, SIZE);
    
    // Initialisation position joueur
    t_position player;
	player.x = 0;
	player.y = 0;

    // Initialisation position sortie
    t_position exit;
	exit.x = 14;
	exit.y = 13;

    movePlayer(maze, SIZE, player);
    runner(maze, player, exit);

    closeOpenAL(&source, &buffer);
    
    return EXIT_SUCCESS;
}