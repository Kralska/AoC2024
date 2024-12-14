#include "point.h"

std::ostream& operator<<(std::ostream& os, Point::Point p) {
  os << "(" << p.x << ", " << p.y << ")";
  return os;
}