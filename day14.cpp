#include <iostream>
#include <ostream>

#include "array2d.h"
#include "point.h"

struct Robot {
  Point::Point start;
  Point::Point velocity;
};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  
  // Task 1 : Safety factor
  int safety_factor = 0;
  std::cout << "Safety factor: " << safety_factor << "\n";
  // Task 2 :
}