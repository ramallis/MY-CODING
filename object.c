#include "object.h"
#include "util.h"
// renders an object form a specified file path
SDL_Texture *render_object(char *object_path, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Texture *object = render_image(window, renderer, object_path);

    return object;
}