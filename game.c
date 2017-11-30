#include "game.h"

GameState *game_init() {
  GameState *state = malloc(sizeof(GameState));
  state->input = input_init();
  return state;
}

void game_deinit(GameState *state) {
  free(state->input);
  free(state);
}

int game_update(GameState *state) {
  /* Poll events and return stop if SDL_QUIT */
  SDL_PollEvent(&state->event);
  if (state->event.type == SDL_QUIT) {
    return 0;
  }

  /* Check game input and return stop if BUTTON_EXIT is pressed */
  input_update(state->input, &state->event);
  if (state->input[BUTTON_EXIT].state) {
    return 0;
  }

  return 1;
}

void game_draw(GameState *state, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(renderer, 100, 100, 200, 200);

  /* Leave a BG color for next clear */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

