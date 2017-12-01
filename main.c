#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char *argv[]) {
  int err, w, h;
  long time, time_last, time_delta;
  SDL_Window *window;
  SDL_Renderer *renderer;

  GameState *game_state;

  /* Screen size */
  w = 640;
  h = 360;

  /* Start SDL subsystems */
  if ((err = SDL_Init (SDL_INIT_VIDEO)) < 0) {
    printf("Failed to initialize SDL\n");
    return err;
  }

  /* Open a window */
  window = SDL_CreateWindow("Space Game",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      w, h, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Failed to create a window. %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  /* Get a rendering context thingy for the window */
  renderer = SDL_CreateRenderer(window, -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("Failed to create a renderer. %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  game_state = game_init(w, h);
  while (game_update(game_state)) {
    SDL_RenderClear(renderer); /* Clear the window */
    game_draw(game_state, renderer);
    SDL_RenderPresent(renderer); /* vsync/update */
  }
  game_deinit(game_state);

  return 0;
}
