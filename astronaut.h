#pragma once
#include "util.h"

// Check position and how many lives astronaut has
typedef struct astronaut {
    int x;
    int y;
    int health;
    SDL_Texture** textures;
} astronaut_t;

// renders the texture of the astronaut, facing the left of the screen
SDL_Texture* render_astronaut_l(SDL_Window* window, SDL_Renderer* renderer);

// renders the texture of the astronaut, facing the right of the screen
SDL_Texture* render_astronaut_r(SDL_Window* window, SDL_Renderer* renderer);
