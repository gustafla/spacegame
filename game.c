#include "game.h"
#include "particle.h"
#include <math.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define pressed(x)    state->input[x].state
#define player_x      state->player->position.x
#define player_y      state->player->position.y

/* Maybe use SDL_Color next time */
static Vec3 const COLOR_PLAYER      = {255, 255, 255};
static Vec3 const COLOR_PLAYER_PJTL = {100, 120, 230};
static Vec3 const COLOR_ENEMY       = {255, 220, 130};
static Vec3 const COLOR_ENEMY_PJTL  = {230, 120, 100};

GameState *game_init(int w, int h) {
  GameState *state = malloc(sizeof(GameState));

  /* initialize input */
  state->input = input_init();

  /* store screen size */
  state->size.x = w;
  state->size.y = h;

  /* construct the player entity */
  state->player = entity_init(w/2, h-20,
      COLOR_PLAYER.x,
      COLOR_PLAYER.y,
      COLOR_PLAYER.z, 3);
  state->player->geometry[0].x = -3;
  state->player->geometry[0].y = 2;
  state->player->geometry[1].x = 0;
  state->player->geometry[1].y = -2;
  state->player->geometry[2].x = 3;
  state->player->geometry[2].y = 2;

  /* player projectiles head */
  state->player_projectiles = malloc(sizeof(LinkedNode));
  state->player_projectiles->next = NULL;
  state->player_projectiles->element = NULL;

  /* enemies head */
  state->enemies = malloc(sizeof(LinkedNode));
  state->enemies->next = NULL;
  state->enemies->element = NULL;

  /* enemy_projectiles head */
  state->enemy_projectiles = malloc(sizeof(LinkedNode));
  state->enemy_projectiles->next = NULL;
  state->enemy_projectiles->element = NULL;

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
  static int score = 0;
  static int player_speed = 3;
  static int enemy_speed = 1;
  static int projectile_speed = 6;
  static int player_shoot_delay = 5;
  static int player_shoot_cldn = 0;
  static int enemy_spawn_delay = 70;
  static int enemy_spawn_cldn = 0;
  static int enemy_shoot_delay = 50;
  static int enemy_shoot_cldn = 0;
  static int enemy_shoot_frame = 1;

  /* Temporary variables */
  int i;
  Particle *projectile;
  Entity *enemy;
  LinkedNode *node_i, *node_j;

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
    projectile = particle_init(player_x, player_y, 0, -projectile_speed,
        &COLOR_PLAYER_PJTL);
    linked_list_add(state->player_projectiles, (void*)projectile);
    player_shoot_cldn = player_shoot_delay;
  }
  if (player_shoot_cldn) { /* if cooldown frames, decrease cooldown counter */
    player_shoot_cldn--;
  }

  /* Update player projectiles */
  for (node_i = state->player_projectiles; node_i != NULL;
      node_i = node_i->next) {
    if (node_i->element != NULL) { /* If contains a particle */
      projectile = (Particle*)node_i->element;
      particle_update(projectile); /* update it's postition */
      /* if OOB */
      if (entity_oob(projectile->entity, state->size.x, state->size.y)) {
        particle_deinit(projectile); /* delete from memory */
        node_i = linked_list_remove(&state->player_projectiles, node_i);
        /* remove it's node and correct the current pointer */
      }
    }
  }

  /* Update enemy projectiles */
  for (node_i = state->enemy_projectiles; node_i != NULL;
      node_i = node_i->next) {
    if (node_i->element != NULL) { /* If contains a particle */
      projectile = (Particle*)node_i->element;
      particle_update(projectile); /* update it's postition */
      /* if OOB */
      if (entity_oob(projectile->entity, state->size.x, state->size.y)) {
        particle_deinit(projectile); /* delete from memory */
        node_i = linked_list_remove(&state->enemy_projectiles, node_i);
        /* remove it's node and correct the current pointer */
        continue;
      }
      if (vec2_distance(&state->player->position,
            &projectile->entity->position) < 4) {
        printf("Game over!\nScore: %d\n", score);
        return 0;
      }
    }
  }

  /* Update enemies and their collisions */
  for (node_i = state->enemies; node_i != NULL; node_i = node_i->next) {
    if (node_i->element != NULL) {
      enemy = (Entity*)node_i->element;
      enemy->position.y += enemy_speed;
      if (enemy->position.y > state->size.y) {
        entity_deinit(enemy);
        node_i = linked_list_remove(&state->enemies, node_i);
        continue;
      }
      /* check for all player projectile collisions */
      for (node_j = state->player_projectiles; node_j != NULL;
          node_j = node_j->next) {
        if (node_j->element != NULL) {
          projectile = (Particle*)node_j->element;
          /* if distance of enemy and projectile is small */
          if (vec2_distance(&projectile->entity->position,
                &enemy->position) < 6) {
            entity_deinit(enemy);
            node_i = linked_list_remove(&state->enemies, node_i);
            particle_deinit(projectile);
            node_j = linked_list_remove(&state->player_projectiles, node_j);
            /* remove both */
            score += 10; /* and add score */
          }
        }
      }
      /* enemy shooting */
      if (enemy_shoot_cldn <= 0 || enemy_shoot_frame) {
        enemy_shoot_frame = 1;
        for (i = 0; i<10; i++) {
          float dx = cos(1./i * M_PI * 2);
          float dy = sin(1./i * M_PI * 2);
          projectile = particle_init(enemy->position.x, enemy->position.y,
              dx*projectile_speed, dy*projectile_speed, &COLOR_ENEMY_PJTL);
          linked_list_add(state->enemy_projectiles, (void*)projectile);
          enemy_shoot_cldn = enemy_shoot_delay;
        }
      }
    }
  }
  enemy_shoot_frame = 0;
  if (enemy_shoot_cldn) {
    enemy_shoot_cldn--;
  }

  /* Spawn enemy */
  if (enemy_spawn_cldn <= 0) {
    enemy = entity_init(rand() % (int)state->size.x, -5,
        COLOR_ENEMY.x,
        COLOR_ENEMY.y,
        COLOR_ENEMY.z, 4);
    enemy->geometry[0].x = -2;
    enemy->geometry[1].x = -2;
    enemy->geometry[2].x = 2;
    enemy->geometry[3].x = 2;
    enemy->geometry[0].y = -2;
    enemy->geometry[1].y = 2;
    enemy->geometry[2].y = 2;
    enemy->geometry[3].y = -2;

    linked_list_add(state->enemies, (void*)enemy);
    enemy_spawn_cldn = enemy_spawn_delay;
  }
  if (enemy_spawn_cldn) {
    enemy_spawn_cldn--;
  }

  return 1;
}

void game_draw(GameState *state, SDL_Renderer *renderer) {
  Particle *projectile;
  Entity *enemy;
  LinkedNode *node;

  entity_draw(state->player, renderer);

  /* Draw player projectiles */
  for (node = state->player_projectiles; node != NULL; node = node->next) {
    if (node->element != NULL) {
      projectile = (Particle*)node->element;
      entity_draw(projectile->entity, renderer);
    }
  }

  /* Draw enemy projectiles */
  for (node = state->enemy_projectiles; node != NULL; node = node->next) {
    if (node->element != NULL) {
      projectile = (Particle*)node->element;
      entity_draw(projectile->entity, renderer);
    }
  }

  /* Draw enemies */
  for (node = state->enemies; node != NULL; node = node->next) {
    if (node->element != NULL) {
      enemy = (Entity*)node->element;
      entity_draw(enemy, renderer);
    }
  }


  /* Leave a BG color for next clear */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

