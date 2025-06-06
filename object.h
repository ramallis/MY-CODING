#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Keeping track of position of object
typedef struct object_pos {
    int x;
    int y;
    int vx;
    int vy;
} object_pos_t;


// Rendering the image of our object
SDL_Texture* render_object(char* object_path, SDL_Window* window, SDL_Renderer* renderer);
