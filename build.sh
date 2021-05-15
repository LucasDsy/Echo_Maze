#!/bin/sh

gcc -g -Wall -Wextra -Wconversion -ansi -std=c11 main.c -o main -lSDL2 -lopenal -lalut
