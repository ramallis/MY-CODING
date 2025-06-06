#include "backgrounds.h"

// renders the interior scenes
SDL_Texture *render_module_1(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Texture *module_1 = render_image(window, renderer, "iss internal module 1.png");
    return module_1;
}

SDL_Texture *render_module_2(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Texture *module_2 = render_image(window, renderer, "iss internal module 2.png");
    return module_2;
}

SDL_Texture *render_module_3(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Texture *module_3 = render_image(window, renderer, "iss internal module 3.png");
    return module_3;
}

// renders the image for the final scene
SDL_Texture *render_outside(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Texture *outside = render_image(window, renderer, "iss internal back.png");
    return outside;
}