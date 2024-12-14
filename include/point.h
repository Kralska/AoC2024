#ifndef _POINT_H_
#define _POINT_H_

#include <ostream>
namespace Point {

struct Point {
  int x, y;
  Point(int x, int y) : x(x), y(y) {}

  auto operator<=>(const Point &) const = default;
};

}

std::ostream &operator<<(std::ostream &os, Point::Point p);

#endif //_POINT_H_