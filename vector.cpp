#include "vector.h"

std::ostream& operator<<(std::ostream& os, vector::Vector p) {
  os << "(" << p.x << ", " << p.y << ")";
  return os;
}