#include "util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// renders an image from a specified file path
SDL_Texture* render_image(SDL_Window* window, SDL_Renderer* renderer, const char *image_file) {
    // Initialize SDL_image
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        // Failed. Log an error, clean up, and exit
        fprintf(stderr, "Failed to initialize SDL_image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Load the image
    SDL_Surface* image_surface = IMG_Load(image_file);
    if (image_surface == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Make texture from loaded surface
    SDL_Texture* image = SDL_CreateTextureFromSurface(renderer, image_surface);
    if (image == NULL) {
    fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    SDL_FreeSurface(image_surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
    }

    // free surface
    SDL_FreeSurface(image_surface);

    return image;
}
