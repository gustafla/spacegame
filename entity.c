#include "entity.h"

Entity *entity_init(int x, int y, int r, int g, int b, int geometry_size) {
  Entity *entity = malloc(sizeof(Entity));

  entity->geometry_size = geometry_size;
  entity->position.x = x;
  entity->position.y = y;
  entity->color.x = r;
  entity->color.y = g;
  entity->color.z = b;

  if (geometry_size) {
    entity->geometry = malloc(sizeof(Vec2) * geometry_size);
  }

  return entity;
}

void entity_deinit(Entity *entity) {
  if (entity->geometry_size) {
    free(entity->geometry);
  }
  free(entity);
}

void entity_draw(Entity *entity, SDL_Renderer *renderer) {
  int i;

  /* Draw with this entity color */
  SDL_SetRenderDrawColor(renderer,
      entity->color.x, entity->color.y, entity->color.z, SDL_ALPHA_OPAQUE);

  if (entity->geometry_size <= 1) { /* If entity doesn't have geometry */
    SDL_RenderDrawLine(renderer, entity->position.x, entity->position.y,
        entity->position.x+1, entity->position.y+1); /* Draw it as small as
                                                        possible */
  } else {
    for (i = 0; i < entity->geometry_size-1; i++) {
      SDL_RenderDrawLine(renderer, 
          entity->position.x + entity->geometry[i].x,
          entity->position.y + entity->geometry[i].y,
          entity->position.x + entity->geometry[i+1].x,
          entity->position.y + entity->geometry[i+1].y); /* Else draw lines */
    }
  }
}

int entity_oob(Entity *entity, int x, int y) {
  return entity->position.x < 0 ||
    entity->position.x > x ||
    entity->position.y < 0 ||
    entity->position.y > y;
}
