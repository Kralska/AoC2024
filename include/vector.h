#ifndef _POINT_H_
#define _POINT_H_

#include <ostream>
namespace vector {

struct Vector {
  int x, y;
  Vector(int x, int y) : x(x), y(y) {}

  auto operator<=>(const Vector &) const = default;

  Vector operator+(const Vector&) const;
  Vector& operator+=(const Vector&);

  Vector operator/(int) const;
  Vector& operator/=(int);
};

}

std::ostream &operator<<(std::ostream &os, vector::Vector p);

#endif //_POINT_H_