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

    // On va chercher le rapport mur / vide
    double ratio = rapportMurVide(maze, player);

    // On modifie le reverb en conséquence
    float yReverb = (float) (d.sud-d.nord)/SIZE;
    float xReverb = (float) (d.est-d.ouest)/SIZE;

    reverb.flReflectionsPan[0] = xReverb;
    reverb.flReflectionsPan[2] = yReverb;
    reverb.flLateReverbPan[0] = (float) xReverb * 1.5f;
    reverb.flLateReverbPan[2] = (float) yReverb * 1.5f;
    reverb.flDecayTime = (float) fabs((xReverb+yReverb) * 10.0) + 0.5;

    printf("Coordonnées joueur : x:%d y:%d d:%d\n", player.x, player.y, player.d);
    printf("ratio : %lf\n", ratio);
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

    while (!(currentKeyStates[SDL_SCANCODE_RETURN] || (player.x == exit.x && player.y == exit.y))) {
		SDL_WaitEvent(&event);
		SDL_PumpEvents();

        if (currentKeyStates[SDL_SCANCODE_UP]) {
        
            if (player.d == NORD && maze[player.x][player.y - 1])
                player.y = MAX(0, player.y - 1);

            else if (player.d == EST && maze[player.x + 1][player.y])
                player.x = MIN(SIZE - 1, player.x + 1);

            else if (player.d == OUEST && maze[player.x - 1][player.y])
                player.x = MAX(0, player.x - 1);

            else if (player.d == SUD && maze[player.x][player.y + 1])
                player.y = MIN(SIZE - 1, player.y + 1);

            action(maze, player);
        
        } else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            
            if (player.d == NORD)
                player.d = OUEST;
                
            else if (player.d == OUEST)
                player.d = SUD;

            else if (player.d == SUD)
                player.d = EST;

            else
                player.d = NORD;

            action(maze, player);
        
        } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            
            if (player.d == NORD)
                player.d = EST;

            else if (player.d == EST)
                player.d = SUD;

            else if (player.d == SUD)
                player.d = OUEST;
                
            else
                player.d = NORD;
                
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
	exit.x = 14;
	exit.y = 13;

    movePlayer(maze, SIZE, player);
    runner(maze, player, exit);

    closeOpenAL(&source, &buffer);
    
    return EXIT_SUCCESS;
}