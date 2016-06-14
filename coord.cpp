#include "coord.hpp"

bool operator==(const Coord &x, const Coord &y)
{
  return x.x == y.x && x.y == y.y;
}
