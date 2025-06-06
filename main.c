// ISS Exploratory Game - Final Project
// A design by Isaac Alexander, Steph Kama-Kama and Sophia Ramalli
// Thanks to Caelan and Almond for their help on the project
// Date: May 15th, 2025

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "astronaut.h"
#include "object.h"
#include "backgrounds.h"
#include "util.h"
#include "alien.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480
#define FRAME_RATE 45
#define OBJECT_SIZE 50
#define IMAGE_WIDTH 720
#define IMAGE_HEIGHT 480


// check to see if the astronaut has collided with any objects
bool check_collision(object_pos_t object_pos, astronaut_t astronaut, const int size) {
    if ((astronaut.x >= object_pos.x) && (astronaut.x <= (object_pos.x + size))) {
        if ((astronaut.y >= object_pos.y) && (astronaut.y <= (object_pos.y + size))) {
            return true;
        }
    } return false;
}

// updates x pos based on dir
void dir_x_update(int dir, object_pos_t *pos) {
    if (dir == 0) {
        pos->x += pos->vx;
    } else {
        pos->x -= pos->vx;
    }
}

// updates y pos based on dir
void dir_y_update(int dir, object_pos_t *pos) {
    if (dir == 0) {
        pos->y += pos->vy;
    } else {
        pos->y -= pos->vy;
    }
}

// changes the x direction of an object, makes it bouce horizontally
void change_dir_x(int *dir, int bound, object_pos_t pos) {
    if (pos.x == bound) {
        if (*dir == 0) {
            *dir = 1;
        } else {
            *dir = 0;
        }
    }
}

// changes the y direction of an object, makes it bounce vertically
void change_dir_y(int *dir, int bound, object_pos_t pos) {
    if (pos.y == bound) {
        if (*dir == 0) {
            *dir = 1;
        } else {
            *dir = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // log the error, quit SDL and exit
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // create SDL window
    SDL_Window *window = SDL_CreateWindow("ISS",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        // log the error, quit SDL and exit
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // initialize the renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        // log the error, quit SDL and exit
        fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // sets up the astonaut
    astronaut_t astronaut = {.x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2, .health = 4};
    astronaut.textures = malloc(sizeof(SDL_Texture*) * 2);
    astronaut.textures[0] = render_astronaut_l(window, renderer);
    astronaut.textures[1] = render_astronaut_r(window, renderer);
    SDL_Texture* curr_astronaut_image = astronaut.textures[1];

    // creating textures for each element
    SDL_Texture *alien = render_alien(window, renderer);
    SDL_Texture *module_1 = render_module_1(window, renderer);
    SDL_Texture *module_2 = render_module_2(window, renderer);
    SDL_Texture *module_3 = render_module_3(window, renderer);
    SDL_Texture *outside = render_outside(window, renderer);
    SDL_Texture *computer_1 = render_object("computer 1.png", window, renderer);
    SDL_Texture *computer_2 = render_object("computer 2.png", window, renderer);
    SDL_Texture *computer_3 = render_object("computer 1.png", window, renderer);
    SDL_Texture *computer_4 = render_object("computer 2.png", window, renderer);
    SDL_Texture *cable_1 = render_object("cable.png", window, renderer);
    SDL_Texture *cable_2 = render_object("cable.png", window, renderer);
    SDL_Texture *cable_3 = render_object("cable.png", window, renderer);

    // sets the initial position for each element (object, astronaut, alien)
    object_pos_t alien_pos = {.x = 600, .y = 240, .vx = 1, .vy = 1};
    object_pos_t computer_1_pos = {.x = 150, .y = 100, .vx = 1, .vy = 1};
    object_pos_t computer_2_pos = {.x = 350, .y = 100, .vx = 1, .vy = 1};
    object_pos_t computer_3_pos = {.x = 400, .y = 100, .vx = 1, .vy = 1};
    object_pos_t computer_4_pos = {.x = 550, .y = 110, .vx = 1, .vy = 1};
    object_pos_t cable_1_pos = {.x = 580, .y = 120, .vx = 1, .vy = 1};
    object_pos_t cable_2_pos = {.x = 200, .y = 130, .vx = 1, .vy = 1};
    object_pos_t cable_3_pos = {.x = 350, .y = 140, .vx = 1, .vy = 1};

    // stores all the movement directions of elements in the x axis (0 is to the right and 1 is to the left)
    int *x_axis_movement = malloc(sizeof(int));

    x_axis_movement[0] = 0;

    // stores all the movement directions of elements in the y axis (0 is down and 1 is up)
    int *y_axis_movement = malloc(sizeof(int) * 2);

    y_axis_movement[0] = 0;
    y_axis_movement[1] = 0;

    int alien_direction = 0;

    int astro_size = 100;

    // sets the current background image
    SDL_Texture *curr_image = module_1;

    bool quit = false;
    bool hit = false;

    // Start the game loop
    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // exits the game when the user clicks the 'X' on the SDL window
                running = false;
                quit = true;
                // collects user input
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    // moves the astronaut up
                    case SDLK_UP:
                        // manages the vertical bounds of the astronaut's movement
                        if (astronaut.y > 140) { 
                            astronaut.y -= 5;
                        } 
                        break;

                    // moves the astronaut down
                    case SDLK_DOWN:
                        // manages the vertical bounds of the astronaut's movement
                        if (astronaut.y <= 270) {
                            astronaut.y += 5;
                        } 
                        break;
                    
                    // moves the astronaut to the left and changes its current displayed texture to the left facing one
                    case SDLK_LEFT:
                    curr_astronaut_image = astronaut.textures[0];
                        // determines the astronaut's position based on which background is rendered
                        // then manages the bounds of the astronaut's movement
                        if (curr_image == module_1) {
                            if (astronaut.x >= 240) {
                                astronaut.x -= 5;
                            }
                        } else {
                            if (astronaut.x >= 50) {
                                astronaut.x -= 5;
                            }
                        }
                        break;

                    
                    // moves the astronaut to the right and changes its current displayed texture to the right facing one
                    case SDLK_RIGHT:
                    curr_astronaut_image = astronaut.textures[1];
                        if (astronaut.x < SCREEN_WIDTH) {
                            astronaut.x += 5;
                        } break;

                    default:
                        break;
                }
            }
        }

        if (curr_image == module_1) { 
            // manages movemets for elements in the first scene
            dir_y_update(y_axis_movement[0], &computer_1_pos);
            dir_x_update(x_axis_movement[0], &computer_1_pos);
            dir_y_update(y_axis_movement[1], &computer_2_pos);
        }

        if (curr_image == module_2) { 
            // manages movements for elements in the second scene
            dir_y_update(y_axis_movement[2], &cable_1_pos);
            dir_x_update(x_axis_movement[1], &cable_1_pos);
            dir_y_update(y_axis_movement[3], &cable_2_pos);
            dir_x_update(x_axis_movement[2], &cable_2_pos);
            dir_y_update(y_axis_movement[4], &cable_3_pos);
            dir_y_update(y_axis_movement[0], &computer_3_pos);
            dir_x_update(x_axis_movement[0], &computer_3_pos);
            dir_y_update(y_axis_movement[1], &computer_4_pos);
        }

        // done if the current background is the last one
        if (curr_image == outside) { 
            // manages movements for elements in the last scene
            if (alien_direction == 0) {
                alien_pos.y += 1;
            } else {
                alien_pos.y -= 1;
            }
        }

        // Fill the screen with white pixels
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // the background's position - does not change over time
        SDL_Rect image_dest = {
            .x = 0,
            .y = 0,
            .w = IMAGE_WIDTH,
            .h = IMAGE_HEIGHT 
        };

        // renders the current background image
        SDL_RenderCopy(renderer, curr_image, NULL, &image_dest);

        // sets the positions of the other elements
        SDL_Rect astronaut_dest = {
            .x = astronaut.x - astro_size / 2,
            .y = astronaut.y - astro_size / 2,
            .w = astro_size,
            .h = astro_size};

        SDL_Rect computer_1_dest = {
            .x = computer_1_pos.x,
            .y = computer_1_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        SDL_Rect computer_2_dest = {
            .x = computer_2_pos.x,
            .y = computer_2_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        SDL_Rect computer_3_dest = {
            .x = computer_3_pos.x,
            .y = computer_3_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        SDL_Rect computer_4_dest = {
            .x = computer_4_pos.x,
            .y = computer_4_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        SDL_Rect cable_1_dest = {
            .x = cable_1_pos.x,
            .y = cable_1_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        SDL_Rect cable_2_dest = {
            .x = cable_2_pos.x,
            .y = cable_2_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        SDL_Rect cable_3_dest = {
            .x = cable_3_pos.x,
            .y = cable_3_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        SDL_Rect alien_dest = {
            .x = alien_pos.x,
            .y = alien_pos.y,
            .w = OBJECT_SIZE,
            .h = OBJECT_SIZE};

        if (curr_image == module_1){
            // checks for collisions in the first scene
            if (check_collision(computer_1_pos, astronaut, OBJECT_SIZE) ||
                check_collision(computer_2_pos, astronaut, OBJECT_SIZE)) {
                if (astro_size != 50) {
                    astro_size -= 1;
                } else {
                    astro_size = 100;
                    astronaut.health -= 1;
                }
            }
        }

        if (curr_image == module_2) {
            // checks for collisions in the second scene
            if (check_collision(computer_3_pos, astronaut, OBJECT_SIZE) ||
                check_collision(computer_4_pos, astronaut, OBJECT_SIZE) ||
                check_collision(cable_1_pos, astronaut, OBJECT_SIZE) ||
                check_collision(cable_2_pos, astronaut, OBJECT_SIZE) ||
                check_collision(cable_3_pos, astronaut, OBJECT_SIZE)) {
                if (astro_size != 50) {
                    astro_size -= 1;
                } else {
                    astro_size = 100;
                    astronaut.health -= 1;
                }
            }
        }

        if (curr_image == outside){
            // checks for collisions in the last scene
            if (check_collision(alien_pos, astronaut, OBJECT_SIZE)) {
                if (astro_size != 50) {
                    astro_size -= 1;
                } else {
                    astro_size = 100;
                    astronaut.health -= 1;
                }
            }
        }

        // manages scene transitions and frees unnecessary textures
        if (astronaut_dest.x >= 600) {
            if (astronaut_dest.y >= 160 && astronaut_dest.y < 320) {
                if (curr_image == module_1) {
                    curr_image = module_2;
                    astronaut.x = (SCREEN_WIDTH / 2) - 200;
                    astronaut.y = (SCREEN_HEIGHT / 2);
                    x_axis_movement = realloc(x_axis_movement, (sizeof(int) * 3));
                    x_axis_movement[0] = 1;
                    x_axis_movement[1] = 0;
                    x_axis_movement[2] = 1;
                    y_axis_movement = realloc(y_axis_movement, (sizeof(int) * 5));
                    y_axis_movement[0] = 0;
                    y_axis_movement[1] = 0;
                    y_axis_movement[2] = 0;
                    y_axis_movement[3] = 0;
                    y_axis_movement[4] = 0;
                    SDL_DestroyTexture(computer_1);
                    SDL_DestroyTexture(computer_2);
                    SDL_DestroyTexture(module_1);
                } else if (curr_image == module_2) {
                    curr_image = module_3;
                    astronaut.x = (SCREEN_WIDTH / 2) - 200;
                    astronaut.y = (SCREEN_HEIGHT / 2);
                    SDL_DestroyTexture(computer_3);
                    SDL_DestroyTexture(computer_4);
                    SDL_DestroyTexture(cable_1);
                    SDL_DestroyTexture(cable_2);
                    SDL_DestroyTexture(cable_3);
                    SDL_DestroyTexture(module_2);
                } else {
                    running = false;
                }
            }
        } else if (astronaut_dest.x >= 400) {
            if (curr_image == module_3) {
                curr_image = outside;
                astronaut.x = (SCREEN_WIDTH / 2) - 200;
                astronaut.y = (SCREEN_HEIGHT / 2);
                SDL_DestroyTexture(module_3);
            }
        }

        if (curr_image == module_1) {

            // bounces the elements of the first scene according to the bounds
            change_dir_y(&y_axis_movement[0], 300, computer_1_pos);
            change_dir_y(&y_axis_movement[0], 99, computer_1_pos);
            change_dir_x(&x_axis_movement[0], 590, computer_1_pos);
            change_dir_x(&x_axis_movement[0], 150, computer_1_pos);
            change_dir_y(&y_axis_movement[1], 300, computer_2_pos);
            change_dir_y(&y_axis_movement[1], 99, computer_2_pos);
        }

        if (curr_image == module_2) {

            // bounces the elements of the second scene according to the bounds
            change_dir_y(&y_axis_movement[2], 300, cable_1_pos);
            change_dir_y(&y_axis_movement[2], 99, cable_1_pos);
            change_dir_x(&x_axis_movement[1], 590, cable_1_pos);
            change_dir_x(&x_axis_movement[1], 50, cable_1_pos);
            change_dir_y(&y_axis_movement[3], 300, cable_2_pos);
            change_dir_y(&y_axis_movement[3], 99, cable_2_pos);
            change_dir_x(&x_axis_movement[2], 590, cable_2_pos);
            change_dir_x(&x_axis_movement[2], 50, cable_2_pos);
            change_dir_y(&y_axis_movement[4], 300, cable_3_pos);
            change_dir_y(&y_axis_movement[4], 99, cable_3_pos);
            change_dir_y(&y_axis_movement[0], 300, computer_3_pos);
            change_dir_y(&y_axis_movement[0], 99, computer_3_pos);
            change_dir_x(&x_axis_movement[0], 590, computer_3_pos);
            change_dir_x(&x_axis_movement[0], 50, computer_3_pos);
            change_dir_y(&y_axis_movement[1], 300, computer_4_pos);
            change_dir_y(&y_axis_movement[1], 99, computer_4_pos);
        }

        if (curr_image == outside) {
            // bounces the alien in the last scene
            if (alien_pos.y == 400) {
                alien_direction = 1;
            }
            else if (alien_pos.y == 100) {
                alien_direction = 0;
            }
        }

        // ends the game if the astronaut has no more health
        if (astronaut.health == 0) {
            running = false;
            
            // memory cleanup
            if (curr_image == module_1) {
                SDL_DestroyTexture(computer_1);
                SDL_DestroyTexture(computer_2);
                SDL_DestroyTexture(module_1);
                SDL_DestroyTexture(computer_3);
                SDL_DestroyTexture(computer_4);
                SDL_DestroyTexture(cable_1);
                SDL_DestroyTexture(cable_2);
                SDL_DestroyTexture(cable_3);
                SDL_DestroyTexture(module_2);
                SDL_DestroyTexture(module_3);
            } else if (curr_image == module_2) {
                SDL_DestroyTexture(computer_3);
                SDL_DestroyTexture(computer_4);
                SDL_DestroyTexture(cable_1);
                SDL_DestroyTexture(cable_2);
                SDL_DestroyTexture(cable_3);
                SDL_DestroyTexture(module_2);
                SDL_DestroyTexture(module_3);
            } else if (curr_image == module_3) {
                SDL_DestroyTexture(module_3);
            }
        }

        // renders the astronaut
        SDL_RenderCopy(renderer, curr_astronaut_image, NULL, &astronaut_dest);

        if (curr_image == module_1) {
            // renders what needs to be in the first scene
            SDL_RenderCopy(renderer, computer_1, NULL, &computer_1_dest);
            SDL_RenderCopy(renderer, computer_2, NULL, &computer_2_dest);
        }

        if (curr_image == module_2) {
            // renders what needs to be in the second scene
            SDL_RenderCopy(renderer, computer_3, NULL, &computer_3_dest);
            SDL_RenderCopy(renderer, computer_4, NULL, &computer_4_dest);
            SDL_RenderCopy(renderer, cable_1, NULL, &cable_1_dest);
            SDL_RenderCopy(renderer, cable_2, NULL, &cable_2_dest);
            SDL_RenderCopy(renderer, cable_3, NULL, &cable_3_dest);
        }

        if (curr_image == outside){
            // renders what needs to be in the last scene
            SDL_RenderCopy(renderer, alien, NULL, &alien_dest);
        }

        // updates the display
        SDL_RenderPresent(renderer);

        // pause before drawing the next frame
        SDL_Delay(1000 / FRAME_RATE);
    }

    // frees remaining elements, if they haven't been freed already
    free(x_axis_movement);
    free(y_axis_movement);
    SDL_DestroyTexture(astronaut.textures[0]);
    SDL_DestroyTexture(astronaut.textures[1]);
    free(astronaut.textures);
    SDL_DestroyTexture(alien);
    SDL_DestroyTexture(outside);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // quits the program
    SDL_Quit();

    // outputs the result in the terminal
    if (astronaut.health == 0) {
        printf("You lost all your lives! Try again.\n");
    } else if (quit) {
        printf("You quit the game!\n");
    } else {
        printf("You won with your health at %d out of 4!\n", astronaut.health);
    }

    return 0;
}