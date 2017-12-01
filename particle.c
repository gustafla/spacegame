#include "particle.h"

Particle *particle_init(int x, int y, int speed_x, int speed_y) {
  Particle *particle = malloc(sizeof(Particle));

  particle->entity = entity_init(x, y, 200, 180, 160, 0);
  particle->speed.x = speed_x;
  particle->speed.y = speed_y;

  return particle;
}

void particle_deinit(Particle *particle) {
  entity_deinit(particle->entity);
  free(particle);
}

void particle_update(Particle *particle) {
  particle->entity->position.x += particle->speed.x;
  particle->entity->position.y += particle->speed.y;
}
