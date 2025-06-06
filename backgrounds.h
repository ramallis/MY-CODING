#pragma once
#include "util.h"

// renders the interior scenes
SDL_Texture *render_module_1(SDL_Window *window, SDL_Renderer *renderer);

SDL_Texture *render_module_2(SDL_Window *window, SDL_Renderer *renderer);

SDL_Texture *render_module_3(SDL_Window *window, SDL_Renderer *renderer);

// renders the image for the final scene
SDL_Texture *render_outside(SDL_Window *window, SDL_Renderer *renderer);