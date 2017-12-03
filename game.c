#include "game.h"
#include "particle.h"

#define pressed(x)    state->input[x].state
#define player_x      state->player->position.x
#define player_y      state->player->position.y

GameState *game_init(int w, int h) {
  GameState *state = malloc(sizeof(GameState));

  /* initialize input */
  state->input = input_init();

  /* store screen size */
  state->size.x = w;
  state->size.y = h;

  /* construct the player entity */
  state->player = entity_init(w/2, h-20, 255, 255, 255, 3);
  state->player->geometry[0].x = -3;
  state->player->geometry[0].y = 2;
  state->player->geometry[1].x = 0;
  state->player->geometry[1].y = -2;
  state->player->geometry[2].x = 3;
  state->player->geometry[2].y = 2;

  /* player projectiles head node */
  state->player_projectiles = malloc(sizeof(LinkedNode));
  state->player_projectiles->next = NULL;
  state->player_projectiles->element = NULL;

  return state;
}

void game_deinit(GameState *state) {
  free(state->input);
  free(state->player_projectiles);
  entity_deinit(state->player);
  free(state);
}

int game_update(GameState *state) {
  /* Game logic variables */
  static int player_speed = 2;
  static int projectile_speed = 6;
  static int player_shoot_delay = 8;
  static int player_shoot_cldn = 0;

  /* Temporary variables */
  Particle *projectile;
  LinkedNode *node;

  /* Poll events and return stop if SDL_QUIT */
  SDL_PollEvent(&state->event);
  if (state->event.type == SDL_QUIT) {
    return 0;
  }

  /* Check game input and return stop if BUTTON_EXIT is pressed */
  input_update(state->input, &state->event);
  if (pressed(BUTTON_EXIT)) {
    return 0;
  }

  /* Process input actions */
  if (pressed(BUTTON_LEFT) && state->player->position.x > 0) {
    state->player->position.x -= player_speed;
  }

  if (pressed(BUTTON_RIGHT) && state->player->position.x < state->size.x) {
    state->player->position.x += player_speed;
  }

  if (pressed(BUTTON_A) && player_shoot_cldn <= 0) {
    projectile = particle_init(player_x, player_y, 0, -projectile_speed);
    linked_list_add(state->player_projectiles, (void*)projectile);
    player_shoot_cldn = player_shoot_delay;
  }
  if (player_shoot_cldn) { /* if cooldown frames, decrease cooldown counter */
    player_shoot_cldn--;
  }

  /* Update player projectiles */
  for (node = state->player_projectiles; node != NULL; node = node->next) {
    if (node->element != NULL) { /* If contains a particle */
      projectile = (Particle*)node->element;
      particle_update(projectile); /* update it's postition */
      /* if OOB */
      if (entity_oob(projectile->entity, state->size.x, state->size.y)) {
        particle_deinit(projectile); /* delete from memory */
        node = linked_list_remove(&state->player_projectiles, node);
        /* remove it's node and correct the current pointer */
      }
    }
  }

  return 1;
}

void game_draw(GameState *state, SDL_Renderer *renderer) {
  Particle *projectile;
  LinkedNode *node;

  entity_draw(state->player, renderer);

  /* Draw player projectiles */
  for (node = state->player_projectiles; node != NULL; node = node->next) {
    if (node->element != NULL) {
      projectile = (Particle*)node->element;
      entity_draw(projectile->entity, renderer);
    }
  }

  /* Leave a BG color for next clear */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

