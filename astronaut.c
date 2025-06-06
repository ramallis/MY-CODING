#include "astronaut.h"

SDL_Texture* render_astronaut_l(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Texture *astronaut_l = render_image(window, renderer, "astronaut_l.png");
    return astronaut_l;
}

SDL_Texture* render_astronaut_r(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Texture *astronaut_r = render_image(window, renderer, "astronaut_r.png");
    return astronaut_r;
}
