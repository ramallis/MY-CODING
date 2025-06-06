#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* render_image(SDL_Window* window, SDL_Renderer* renderer, const char *image_file);