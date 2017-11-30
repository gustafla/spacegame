#include "game.h"

int update_game(GameState *state) {
  /* Poll events and return stop if SDL_QUIT */
  SDL_PollEvent(&state->event);
  if (state->event.type == SDL_QUIT) {
    return 0;
  }



  return 1;
}

void draw_game(GameState *state, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(renderer, 100, 100, 200, 200);

  /* Leave a BG color for next clear */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

