#include "alien.h"

SDL_Texture *render_alien(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Texture *alien = render_image(window, renderer, "alien.png");
    return alien;
}
