#include "vec2.h"
#include <math.h>

float vec2_distance(Vec2 *a, Vec2 *b) {
  float x = a->x - b->x;
  float y = a->y - b->y;
  return sqrt(x*x + y*y);
}
