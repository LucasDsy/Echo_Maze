#include "../headers/sdl.h"
#include "../headers/openal.h"

/** SDL **/
SDL_Window *pWindow = NULL;
SDL_Renderer *pRenderer = NULL;
SDL_Surface *mazeDisplay = NULL;

const int gridCellSize = 50;

int init_sdl(int DIM)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			     "[DEBUG] > %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (SDL_CreateWindowAndRenderer
	    ((DIM * gridCellSize) + 1, (DIM * gridCellSize) + 1, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			     "[DEBUG] > %s", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	} else {
        mazeDisplay = SDL_GetWindowSurface(pWindow);
    }
    
    SDL_SetWindowTitle(pWindow, "Maze");

    return EXIT_SUCCESS;
}

void init_grid(int DIM, int **maze)
{
    int windowWidth = (DIM * gridCellSize) + 1;
    int windowHeight = (DIM * gridCellSize) + 1;
    int x, y = 0;

    SDL_Rect grid_cursor = {
        .x = 0,
        .y = 0,
        .w = gridCellSize,
        .h = gridCellSize,
    };

    SDL_Color backgroundColor = {22, 22, 22, 255};
    SDL_Color lineColor = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(pRenderer, backgroundColor.r, backgroundColor.g,
                               backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(pRenderer);

    SDL_SetRenderDrawColor(pRenderer, lineColor.r, lineColor.g,
                               lineColor.b, lineColor.a);

    for(x = 0; x < windowWidth; x += gridCellSize) {
        SDL_RenderDrawLine(pRenderer, x, 0, x, windowWidth);
    }

    for(y = 0; y < windowHeight; y += gridCellSize) {
        SDL_RenderDrawLine(pRenderer, 0, y, windowHeight, y);
    }

    SDL_SetRenderDrawColor(pRenderer, lineColor.r,
                           lineColor.g, lineColor.b,
                           lineColor.a);

    // Draw Maze Walls
    for(x = 0; x < DIM; x++) {
        for(y = 0; y < DIM; y++) {
            grid_cursor.x = x * gridCellSize;
            grid_cursor.y = y * gridCellSize;
            if(maze[x][y])
                SDL_RenderFillRect(pRenderer, &grid_cursor);
        }
    }
}

void animate_player(int DIM, int **maze, t_player player) {

    init_grid(DIM, maze);
    //play_sound();
    SDL_Color playerColor = {0, 255, 0, 255}; // green

    SDL_SetRenderDrawColor(pRenderer, playerColor.r,
                           playerColor.g, playerColor.b,
                           playerColor.a);

    SDL_Rect gridPlayer = {
        .x = player.x * gridCellSize,
        .y = player.y * gridCellSize,
        .w = gridCellSize,
        .h = gridCellSize,
    };

    SDL_RenderFillRect(pRenderer, &gridPlayer);
    SDL_RenderPresent(pRenderer);
}

int runner(int DIM, int **maze, t_player player, t_exit exit) {
    
    static SDL_Event event;
    static const Uint8 *currentKeyStates = NULL;
	currentKeyStates = SDL_GetKeyboardState(NULL);

    while (!(event.type == SDL_QUIT
            || (player.x == exit.x && player.y == exit.y))) {
		SDL_WaitEvent(&event);
		SDL_PumpEvents();
		if (currentKeyStates[SDL_SCANCODE_UP] 
                && maze[player.x][player.y - 1]) {
            player.y = MAX(0, player.y - 1);
            player.d = 1;
            play_source();
            animate_player(DIM, maze, player);
		} else if (currentKeyStates[SDL_SCANCODE_DOWN] 
                && maze[player.x][player.y + 1]) {
			player.y = MIN(DIM - 1, player.y + 1);
            player.d = 2;
			animate_player(DIM, maze, player);
            play_source();
		} else if (currentKeyStates[SDL_SCANCODE_LEFT]
			    && maze[player.x - 1][player.y]) {
			player.x = MAX(0, player.x - 1);
            player.d = 3;
			animate_player(DIM, maze, player);
            play_source();
		} else if (currentKeyStates[SDL_SCANCODE_RIGHT]
			    && maze[player.x + 1][player.y]) {
			player.x = MIN(DIM - 1, player.x + 1);
            player.d = 4;
			animate_player(DIM, maze, player);
            play_source();
		}
	}

    return 0;
}