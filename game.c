#include "game.h"

int update_game(GameState *state) {
  SDL_PollEvent(&state->event);
  if (state->event.type == SDL_QUIT) {
    return 0;
  }
  return 1;
}

void draw_game(GameState *state, SDL_Renderer *renderer) {

}

