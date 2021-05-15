#!/bin/sh

gcc -g -Wall -Wextra -Wconversion -ansi -std=c11 \
    main.c sources/maze.c sources/openal.c sources/sdl.c \
    -o main -lSDL2 -lopenal -lalut
