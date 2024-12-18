#include "shortest_path.h"
#include "array2d.h"

#include <algorithm>
#include <climits>
#include <map>

enum class Direction : int {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3,
  NONE = 4
};

std::vector<paths::Point> paths::default_get_neighbors(Point center, std::size_t width , std::size_t height) {
  std::vector<Point> neighbors{};
  if (center.x > 0) { // Left
    neighbors.push_back({center.x - 1, center.y});
  }
  if (center.x + 1 < width) { // Right
    neighbors.push_back({center.x + 1, center.y});
  }
  if (center.y > 0) { // Up
    neighbors.push_back({center.x, center.y - 1});
  }
  if (center.y + 1 < height) { // Down
    neighbors.push_back({center.x, center.y + 1});
  }
  return neighbors;
}

paths::Dijkstra::Dijkstra(const array2D::Array2D<int> &maze, Point from)
    : maze_(maze), from_(from), costs_(maze.length(), maze.height(), INT_MAX) {
  calculate_shortest_paths(from_);
}

void paths::Dijkstra::calculate_shortest_paths(Point from) {
  from_ = from;
  costs_.at(from_.x, from_.y) = 0;

  std::multimap<int, Point, std::greater<>> queue{{0, from}};

  while (!queue.empty()) {
    auto iter = --queue.end();
    auto current = *iter;
    queue.erase(iter);

    std::vector<Point> neighbors = get_neighbors(current.second);
    for (const auto &neighbor : neighbors) {
      if (is_wall_at(neighbor.x, neighbor.y)) {
        int cost = current.first + 1;
        if (cost < costs_.at(neighbor.x, neighbor.y)) {
          costs_.at(neighbor.x, neighbor.y) = cost;
          queue.insert({cost, neighbor});
        }
      }
    }
  }
}

std::vector<paths::Point> paths::Dijkstra::get_shortest_path(Point to) {
  std::vector<Point> rpath{to};
  Point current = to;
  while(current != from_){
    auto neighbors = get_neighbors(current);
    for(const auto& neighbor : neighbors){
      if(costs_.at(current.x, current.y) - 1 == costs_.at(neighbor.x, neighbor.y)){
        rpath.push_back(neighbor);
        current = neighbor;
        break;
      }
    }
  }
  std::ranges::reverse(rpath);
  return rpath;
}

void paths::Dijkstra::print(std::function<std::string(int)> int_to_char) {
  array2D::print_array(maze_, int_to_char);
}

std::string paths::Dijkstra::default_int_to_string(int i) { return i == 0 ? "#" : " "; }

bool paths::Dijkstra::is_wall_at(std::size_t x, std::size_t y) {
  return maze_.cat(x, y) != 0;
}

int paths::Dijkstra::get_shortest_path_length(Point to) {
  return costs_.at(to.x, to.y);
}
