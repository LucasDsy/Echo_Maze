#include <SDL2/SDL.h>

#define GRAPHICALCELLSIZE 50


// Variables globales SDL
SDL_Window* pWindow = NULL;
SDL_Renderer* pRenderer = NULL;
SDL_Surface* mazeDisplay = NULL;


int initSDL(int size) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (SDL_CreateWindowAndRenderer
	    ((size * GRAPHICALCELLSIZE) + 1, (size * GRAPHICALCELLSIZE) + 1, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;

	} else {
        mazeDisplay = SDL_GetWindowSurface(pWindow);
    }
    
    SDL_SetWindowTitle(pWindow, "Maze");

    return EXIT_SUCCESS;
}


void initGrid(int** maze, int size) {

    int windowWidth = (size * GRAPHICALCELLSIZE) + 1;
    int windowHeight = (size * GRAPHICALCELLSIZE) + 1;

    SDL_Rect grid_cursor = {
        .x = 0,
        .y = 0,
        .w = GRAPHICALCELLSIZE,
        .h = GRAPHICALCELLSIZE,
    };

    SDL_Color backgroundColor = {22, 22, 22, 255};
    SDL_Color lineColor = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(pRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(pRenderer);

    SDL_SetRenderDrawColor(pRenderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);

    for (int i = 0; i < windowWidth; i += GRAPHICALCELLSIZE) {
        SDL_RenderDrawLine(pRenderer, i, 0, i, windowWidth);
        SDL_RenderDrawLine(pRenderer, 0, i, windowHeight, i);
    }
        

    SDL_SetRenderDrawColor(pRenderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);

    // On dessine les murs
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            grid_cursor.x = x * GRAPHICALCELLSIZE;
            grid_cursor.y = y * GRAPHICALCELLSIZE;

            if (maze[x][y])
                SDL_RenderFillRect(pRenderer, &grid_cursor);
        }
    }
}


void movePlayer(int** maze, int size, t_position player) {

    // On réinitialise la grille à chaque fois
    initGrid(maze, size);

    // Couleur verte sur la position du joueur
    SDL_Color playerColor = {0, 255, 0, 255};

    SDL_SetRenderDrawColor(pRenderer, playerColor.r, playerColor.g, playerColor.b, playerColor.a);

    SDL_Rect gridPlayer = {
        .x = player.x * GRAPHICALCELLSIZE,
        .y = player.y * GRAPHICALCELLSIZE,
        .w = GRAPHICALCELLSIZE,
        .h = GRAPHICALCELLSIZE,
    };

    SDL_RenderFillRect(pRenderer, &gridPlayer);
    SDL_RenderPresent(pRenderer);
}