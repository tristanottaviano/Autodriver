#pragma once
#include <SDL2/SDL.h>  

Uint32 getpixel(SDL_Surface* surface, int x, int y);
SDL_Color decodeColor(SDL_Surface* gSurface, int x, int y);
bool compareColor(SDL_Color c1, SDL_Color c2);

