#ifndef _SHORTEST_PATH_H_
#define _SHORTEST_PATH_H_

#include "array2d.h"
#include "vector.h"
#include <cstddef>

namespace paths {

using Point = vector::Vector;

std::vector<Point> default_get_neighbors(Point center, std::size_t width , std::size_t height);

class Dijkstra {
public:
  Dijkstra(const array2D::Array2D<int>&, Point from);

  void calculate_shortest_paths(Point from);
  std::vector<Point> get_shortest_path(Point to);
  int get_shortest_path_length(Point to);
  std::vector<Point> get_shortest_path(Point from, Point to);
  

  const array2D::Array2D<int> &get_costs() { return costs_; }

  static const int kWALL = 0;
  static const int kAIR = 1;

  void print(std::function<std::string(int)> = default_int_to_string);
private:
  

  Point from_;
  /**
   * The following rules apply for the maze:
   * at(x, y) == 0 => WALL(impassable terrain)
   * at(x, y) != 0 => AIR (  passable terrain)
   */
  const array2D::Array2D<int> maze_;
  array2D::Array2D<int> costs_;

  static std::string default_int_to_string(int);
  std::function<std::vector<Point>(Point)> get_neighbors = std::bind(default_get_neighbors, std::placeholders::_1, maze_.length(), maze_.height());

  bool is_wall_at(std::size_t x, std::size_t y);
};


} // namespace paths

#endif // _SHORTEST_PATH_H_