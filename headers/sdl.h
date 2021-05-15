#include <SDL2/SDL.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)<(b))?(b):(a))

typedef struct {
	int x, y;
    int d;
} t_player;

typedef struct {
	int x, y;
} t_exit;

int init_sdl(int DIM);
void init_grid(int DIM, int *maze[]);
void animate_player(int DIM, int *maze[], t_player player);
int runner(int DIM, int *maze[], t_player player, t_exit exit);
