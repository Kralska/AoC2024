#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <type_traits>

#include "array2d.h"

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

enum class Tile : int { Empty = 0, Wall = 1, Start = 2, End = 3 };

class TileArray : public array2D::Array2D<int> {
public:
  TileArray(std::size_t width, std::size_t height,
            Tile defaultTile = Tile::Empty)
      : array2D::Array2D<int>(width, height, static_cast<int>(defaultTile)) {}

  TileArray(const array2D::Array2D<int> &arr) : array2D::Array2D<int>(arr) {}

  Tile get_tile_at(std::size_t x, std::size_t y) const {
    auto at_pos = cat(x, y);
    assert((0 <= at_pos && at_pos <= 3));
    return static_cast<Tile>(at_pos);
  }

  void set_tile_at(std::size_t x, std::size_t y, Tile tile) {
    at(x, y) = static_cast<int>(tile);
  }
};

void print(const TileArray &warehouse) {
  std::function<char(int)> to_char = [](const int &v) -> char {
    const static std::map<int, char> tile_to_char{
        {static_cast<int>(Tile::Wall), '#'},
        {static_cast<int>(Tile::Empty), ' '},
        {static_cast<int>(Tile::Start), 'S'},
        {static_cast<int>(Tile::End), 'E'},
    };
    return tile_to_char.at(v);
  };
  array2D::print_array(warehouse, to_char);
}

// ===== Read Input =====
TileArray read_reindeer_maze(std::string file_location) {
  std::function<int(char)> ctoi = [](char c) {
    const static std::map<char, Tile> char_to_tile{
        {'#', Tile::Wall},
        {'.', Tile::Empty},
        {'S', Tile::Start},
        {'E', Tile::End},
    };
    return static_cast<int>(char_to_tile.at(c));
  };
  array2D::Array2D<int> array = array2D::read_array(file_location, ctoi);
  return TileArray(array);
}

enum class Direction : int { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3, NONE = 4 };

std::ostream &operator<<(std::ostream &os, Direction dir) {
  if (dir == Direction::NORTH) {
    os << "North";
  } else if (dir == Direction::EAST) {
    os << "East";
  } else if (dir == Direction::SOUTH) {
    os << "South";
  } else if (dir == Direction::WEST) {
    os << "West";
  } else if (dir == Direction::NONE) {
    os << "None";
  }
  return os;
}

struct Location {
  Direction direction;
  std::size_t x, y;

  Location(std::size_t x, std::size_t y, Direction direction)
      : x(x), y(y), direction(direction) {}
};

std::ostream &operator<<(std::ostream &os, const Location &loc) {
  os << "{" << loc.x << ", " << loc.y << ", ";
  os << loc.direction;
  os << "}";
  return os;
}

int dijkstra_pathing(const TileArray &reindeer_maze, std::size_t start_x,
                     std::size_t start_y, std::size_t end_x,
                     std::size_t end_y) {
  std::multimap<int, Location, std::greater<>> queue{};
  queue.insert({0, {start_x, start_y, Direction::EAST}});

  array2D::Array2D<int> costs{reindeer_maze.length(), reindeer_maze.height(),
                              INT_MAX - 2000};
  costs.at(start_x, start_y) = 0;
  array2D::Array2D<int> coming_from {reindeer_maze.length(), reindeer_maze.height(), static_cast<int>(Direction::NONE)};

  while (!queue.empty()) {
    auto iter = --queue.end();
    auto current = *iter;
    queue.erase(iter);
    // North
    if (reindeer_maze.get_tile_at(current.second.x, current.second.y - 1) !=
        Tile::Wall) {
      int cost = current.first + 1;
      if (current.second.direction != Direction::NORTH) {
        cost += 1000;
      }
      Location loc{current.second.x, current.second.y - 1, Direction::NORTH};
      if (cost < costs.at(loc.x, loc.y) + (coming_from.at(loc.x, loc.y) == static_cast<int>(Direction::SOUTH) ? 0 : 1000)) {
        costs.at(loc.x, loc.y) = cost;
        coming_from.at(loc.x, loc.y) = static_cast<int>(Direction::SOUTH);
        queue.insert({cost, loc});
      }
    }
    // East
    if (reindeer_maze.get_tile_at(current.second.x + 1, current.second.y) !=
        Tile::Wall) {
      int cost = current.first + 1;
      if (current.second.direction != Direction::EAST) {
        cost += 1000;
      }
      Location loc{current.second.x + 1, current.second.y, Direction::EAST};
      if (cost < costs.at(loc.x, loc.y) + (coming_from.at(loc.x, loc.y) == static_cast<int>(Direction::WEST) ? 0 : 1000)) {
        costs.at(loc.x, loc.y) = cost;
        coming_from.at(loc.x, loc.y) = static_cast<int>(Direction::WEST);
        queue.insert({cost, loc});
      }
    }
    // South
    if (reindeer_maze.get_tile_at(current.second.x, current.second.y + 1) !=
        Tile::Wall) {
      int cost = current.first + 1;
      if (current.second.direction != Direction::SOUTH) {
        cost += 1000;
      }
      Location loc{current.second.x, current.second.y + 1, Direction::SOUTH};
      if (cost < costs.at(loc.x, loc.y) + (coming_from.at(loc.x, loc.y) == static_cast<int>(Direction::NORTH) ? 0 : 1000)) {
        costs.at(loc.x, loc.y) = cost;
        coming_from.at(loc.x, loc.y) = static_cast<int>(Direction::NORTH);
        queue.insert({cost, loc});
      }
    }
    // West
    if (reindeer_maze.get_tile_at(current.second.x - 1, current.second.y) !=
        Tile::Wall) {
      int cost = current.first + 1;
      if (current.second.direction != Direction::WEST) {
        cost += 1000;
      }
      Location loc{current.second.x - 1, current.second.y, Direction::WEST};
      if (cost < costs.at(loc.x, loc.y) + (coming_from.at(loc.x, loc.y) == static_cast<int>(Direction::EAST) ? 0 : 1000)) {
        costs.at(loc.x, loc.y) = cost;
        coming_from.at(loc.x, loc.y) = static_cast<int>(Direction::EAST);
        queue.insert({cost, loc});
      }
    }
  }
  return costs.at(end_x, end_y);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  TileArray reindeer_maze = read_reindeer_maze(argv[1]);
  std::size_t start_x = 1, start_y = reindeer_maze.height() - 2;
  std::size_t end_x = reindeer_maze.length() - 2, end_y = 1;

  // Task 1 : Score from Start to End
  int total_score =
      dijkstra_pathing(reindeer_maze, start_x, start_y, end_x, end_y);
  std::cout << "Total score: " << total_score << "\n";
}