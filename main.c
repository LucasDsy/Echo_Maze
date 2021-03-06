#include "headers/sdl.h"
#include "headers/openal.h"
#include "headers/maze.h"
#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)<(b))?(b):(a))

// OpenAL global variables
ALuint source;
ALuint buffer;
EFXEAXREVERBPROPERTIES reverb;


void action(int** maze, t_position player) {
    // On bouge le joueur à sa nouvelle position
    movePlayer(maze, SIZE, player);

    // On rectifie l'orientation du joueur
    //setOrientation((int)player.d);

    // On va chercher la distance entre le joueur et chaque mur
    distances d = distancesToWall(maze, player);

    // On modifie le reverb en conséquence
    float yReverb = (float) (d.sud-d.nord)/SIZE;
    float xReverb = (float) (d.est-d.ouest)/SIZE;

    reverb.flReflectionsPan[0] = xReverb  * 1.1f;
    reverb.flReflectionsPan[2] = yReverb * 1.1f;
    reverb.flLateReverbPan[0] = (float) xReverb * 1.5f;
    reverb.flLateReverbPan[2] = (float) yReverb * 1.5f;
    reverb.flDecayTime = ((float) (d.nord + d.sud + d.est + d.ouest) * 20.0f / SIZE) + 0.1f;

    printf("Coordonnées joueur : x:%d y:%d d:%d\n", player.x, player.y, player.d);
    printf("reflexionPan Sud-Nord : %lf\n", reverb.flReflectionsPan[0]);
    printf("reflexionPan Est-Ouest : %lf\n", reverb.flReflectionsPan[2]);
    printf("reflexionPan tardive axe X : %lf\n", reverb.flLateReverbPan[0]);
    printf("reflexionPan tardive axe Y : %lf\n\n", reverb.flLateReverbPan[2]);
    printf("decay time : %lf\n\n", reverb.flDecayTime);
    
    // On joue le son avec le reverb approprié
    playSourceWithReverb(source, reverb);
}


void runner(int** maze, t_position player, t_position exit) {
    
    static SDL_Event event;
    static const Uint8 *currentKeyStates = NULL;
	currentKeyStates = SDL_GetKeyboardState(NULL);

    while (!(currentKeyStates[SDL_SCANCODE_RETURN]) && !(player.x == exit.x && player.y == exit.y)) {
		SDL_WaitEvent(&event);
		SDL_PumpEvents();
        
		if (currentKeyStates[SDL_SCANCODE_UP] && maze[player.x][player.y - 1]) {
            player.y = MAX(0, player.y - 1);
            player.d = NORD;
            action(maze, player);
        }

		else if (currentKeyStates[SDL_SCANCODE_DOWN] && maze[player.x][player.y + 1]) {
			player.y = MIN(SIZE - 1, player.y + 1);
            player.d = SUD;
            action(maze, player);
        }

		else if (currentKeyStates[SDL_SCANCODE_LEFT] && maze[player.x - 1][player.y]) {
			player.x = MAX(0, player.x - 1);
            player.d = OUEST;
            action(maze, player);
        }

		else if (currentKeyStates[SDL_SCANCODE_RIGHT] && maze[player.x + 1][player.y]) {
			player.x = MIN(SIZE - 1, player.x + 1);
            player.d = EST;
            action(maze, player);
        }
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
	exit.x = 13;
	exit.y = 14;

    movePlayer(maze, SIZE, player);
    runner(maze, player, exit);

    closeOpenAL(&source, &buffer);
    
    return EXIT_SUCCESS;
}