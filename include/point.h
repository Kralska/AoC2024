#ifndef _POINT_H_
#define _POINT_H_

#include <cstddef>
#include <ostream>
namespace Point {

struct Point {
  std::size_t x, y;
  Point(std::size_t x, std::size_t y) : x(x), y(y) {}

  auto operator<=>(const Point &) const = default;
};

}

std::ostream &operator<<(std::ostream &os, Point::Point p);

#endif //_POINT_H_