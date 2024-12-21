#include "vector.h"

std::ostream& operator<<(std::ostream& os, vector::Vector p) {
  os << "(" << p.x << ", " << p.y << ")";
  return os;
}

vector::Vector vector::Vector::operator+(const Vector& other) const {
  return {x + other.x, y + other.y};
}

vector::Vector& vector::Vector::operator+=(const Vector& other) {
  x += other.x;
  y += other.y;
  return *this;
}

vector::Vector vector::Vector::operator/(int i) const {
  return {x / i, y / i};
}

vector::Vector& vector::Vector::operator/=(int i) {
  x /= i;
  y /= i;
  return *this;
}