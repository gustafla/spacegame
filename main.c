#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char *argv[]) {
  int err=0;
  SDL_Window *window;
  SDL_Surface *surface;

  GameState *game_state;

  if ((err = SDL_Init (SDL_INIT_VIDEO)) < 0) {
    printf("Failed to initialize SDL\n");
  } else {
    window = SDL_CreateWindow("Space Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Failed to create a window. %s\n", SDL_GetError());
      err = EXIT_FAILURE;
    } else {
      surface = SDL_GetWindowSurface(window);
    }
  }

  if (err != 0)
    return err;

  while (update_game(game_state)) {
    SDL_FillRect(surface, NULL, 0);
    draw_game(game_state, surface);
    SDL_UpdateWindowSurface(window);
  }

  return 0;
}
