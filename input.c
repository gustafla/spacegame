#include "input.h"

InputButton *input_init() {
  InputButton *input = malloc(sizeof(InputButton)*BUTTON_COUNT);
  if (input == NULL) {
    return NULL;
  }
  memset(input, 0, sizeof(InputButton)*BUTTON_COUNT);

  input[0].key = SDLK_x;
  input[1].key = SDLK_z;
  input[2].key = SDLK_UP;
  input[3].key = SDLK_DOWN;
  input[4].key = SDLK_LEFT;
  input[5].key = SDLK_RIGHT;
  input[6].key = SDLK_RETURN;
  input[7].key = SDLK_ESCAPE;

  return input;
}

void input_update(InputButton input[], SDL_Event *event) {
  int i;
  switch (event->type) {
    case SDL_KEYDOWN:
      for (i=0; i<BUTTON_COUNT; i++) {
        if (event->key.keysym.sym == input[i].key) {
          input[i].state=1;
        }
      }
      break;
    case SDL_KEYUP:
      for (i=0; i<BUTTON_COUNT; i++) {
        if (event->key.keysym.sym == input[i].key) {
          input[i].state=0;
        }
      }
      break;
  }
}
