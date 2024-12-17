#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

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
  std::function<std::string(int)> to_char = [](const int &v) -> std::string {
    const static std::map<int, char> tile_to_char{
        {static_cast<int>(Tile::Wall), '#'},
        {static_cast<int>(Tile::Empty), ' '},
        {static_cast<int>(Tile::Start), 'S'},
        {static_cast<int>(Tile::End), 'E'},
    };
    return std::to_string(tile_to_char.at(v));
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

enum class Direction : int {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3,
  NONE = 4
};

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

  auto operator<=>(const Location &) const = default;
};

std::ostream &operator<<(std::ostream &os, const Location &loc) {
  os << "{" << loc.x << ", " << loc.y << ", ";
  os << loc.direction;
  os << "}";
  return os;
}

class DijkstraPathing {
public:
  DijkstraPathing(const TileArray &reindeer_maze, Location start, Location end)
      : start_(start), end_({end.x, end.y, Direction::NONE}),
        maze_(reindeer_maze),
        costs_(maze_.length(), maze_.height(), INT_MAX - 2000),
        coming_from_(maze_.length(), maze_.height(),
                     static_cast<int>(Direction::NONE)) {
    costs_.at(start_.x, start_.y) = 0;
    path_length_ = find_path_length();
  }

  int find_path_length() {
    std::multimap<int, Location, std::greater<>> queue{{0, start_}};

    while (!queue.empty()) {
      auto iter = --queue.end();
      auto current = *iter;
      queue.erase(iter);
      // North
      if (maze_.get_tile_at(current.second.x, current.second.y - 1) !=
          Tile::Wall) {
        int cost = current.first + 1;
        if (current.second.direction != Direction::NORTH) {
          cost += 1000;
        } else { // Skip crosses (+)
          if (maze_.get_tile_at(current.second.x, current.second.y - 2) !=
              Tile::Wall) {
            if (cost + 1 < costs_.at(current.second.x, current.second.y - 2)) {
              costs_.at(current.second.x, current.second.y - 2) = cost + 1;
              coming_from_.at(current.second.x, current.second.y - 2) =
                  static_cast<int>(Direction::SOUTH);
              queue.insert(
                  {cost + 1,
                   {current.second.x, current.second.y - 2, Direction::NORTH}});
            }
          }
        }
        Location loc{current.second.x, current.second.y - 1, Direction::NORTH};
        if (cost < costs_.at(loc.x, loc.y)) {
          costs_.at(loc.x, loc.y) = cost;
          coming_from_.at(loc.x, loc.y) = static_cast<int>(Direction::SOUTH);
          queue.insert({cost, loc});
        }
      }
      // East
      if (maze_.get_tile_at(current.second.x + 1, current.second.y) !=
          Tile::Wall) {
        int cost = current.first + 1;
        if (current.second.direction != Direction::EAST) {
          cost += 1000;
        } else { // Skip crosses (+)
          if (maze_.get_tile_at(current.second.x + 2, current.second.y) !=
              Tile::Wall) {
            if (cost + 1 < costs_.at(current.second.x + 2, current.second.y)) {
              costs_.at(current.second.x + 2, current.second.y) = cost + 1;
              coming_from_.at(current.second.x + 2, current.second.y) =
                  static_cast<int>(Direction::WEST);
              queue.insert(
                  {cost + 1,
                   {current.second.x + 2, current.second.y, Direction::EAST}});
            }
          }
        }
        Location loc{current.second.x + 1, current.second.y, Direction::EAST};
        if (cost < costs_.at(loc.x, loc.y)) {
          costs_.at(loc.x, loc.y) = cost;
          coming_from_.at(loc.x, loc.y) = static_cast<int>(Direction::WEST);
          queue.insert({cost, loc});
        }
      }
      // South
      if (maze_.get_tile_at(current.second.x, current.second.y + 1) !=
          Tile::Wall) {
        int cost = current.first + 1;
        if (current.second.direction != Direction::SOUTH) {
          cost += 1000;
        } else { // Skip crosses (+)
          if (maze_.get_tile_at(current.second.x, current.second.y + 2) !=
              Tile::Wall) {
            if (cost + 1 < costs_.at(current.second.x, current.second.y + 2)) {
              costs_.at(current.second.x, current.second.y + 2) = cost + 1;
              coming_from_.at(current.second.x, current.second.y + 2) =
                  static_cast<int>(Direction::NORTH);
              queue.insert(
                  {cost + 1,
                   {current.second.x, current.second.y + 2, Direction::SOUTH}});
            }
          }
        }
        Location loc{current.second.x, current.second.y + 1, Direction::SOUTH};
        if (cost < costs_.at(loc.x, loc.y)) {
          costs_.at(loc.x, loc.y) = cost;
          coming_from_.at(loc.x, loc.y) = static_cast<int>(Direction::NORTH);
          queue.insert({cost, loc});
        }
      }
      // West
      if (maze_.get_tile_at(current.second.x - 1, current.second.y) !=
          Tile::Wall) {
        int cost = current.first + 1;
        if (current.second.direction != Direction::WEST) {
          cost += 1000;
        } else { // Skip crosses (+)
          if (maze_.get_tile_at(current.second.x - 2, current.second.y) !=
              Tile::Wall) {
            if (cost + 1 < costs_.at(current.second.x - 2, current.second.y)) {
              costs_.at(current.second.x - 2, current.second.y) = cost + 1;
              coming_from_.at(current.second.x - 2, current.second.y) =
                  static_cast<int>(Direction::EAST);
              queue.insert(
                  {cost + 1,
                   {current.second.x - 2, current.second.y, Direction::WEST}});
            }
          }
        }
        Location loc{current.second.x - 1, current.second.y, Direction::WEST};
        if (cost < costs_.at(loc.x, loc.y)) {
          costs_.at(loc.x, loc.y) = cost;
          coming_from_.at(loc.x, loc.y) = static_cast<int>(Direction::EAST);
          queue.insert({cost, loc});
        }
      }
    }
    return costs_.at(end_.x, end_.y);
  }

  int get_path_length() { return path_length_; };
  const array2D::Array2D<int> &get_costs() { return costs_; }
  const array2D::Array2D<int> &get_coming_from() { return coming_from_; }

  std::set<Location> get_optimal_seating_spots() {
    std::set<Location> optimal_seating_spots{end_};
    std::vector<Location> queue{end_};
    std::set<Location> done {};

    while (!queue.empty()) {
      auto iter = --queue.end();
      auto current = *iter;
      queue.erase(iter);

      if(done.contains(current)){
        continue;
      }
      done.insert(current);
      // North
      std::size_t x = current.x;
      std::size_t y = current.y - 1;
      if (maze_.get_tile_at(x, y) != Tile::Wall) {
        if (coming_from_.at(current.x, current.y) == coming_from_.at(x, y) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 1) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        } else if (coming_from_.at(current.x, current.y) !=
                       coming_from_.at(x, y) &&
                   costs_.at(current.x, current.y) == costs_.at(x, y) + 1001) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
        x = current.x;
        y = current.y - 2;
        if (coming_from_.at(current.x, current.y) ==
                static_cast<int>(Direction::NORTH) &&
            coming_from_.at(x, y) == static_cast<int>(Direction::NORTH) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 2) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y + 1, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
      }

      // East
      x = current.x + 1;
      y = current.y;
      if (maze_.get_tile_at(x, y) != Tile::Wall) {
        if (coming_from_.at(current.x, current.y) == coming_from_.at(x, y) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 1) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        } else if (coming_from_.at(current.x, current.y) !=
                       coming_from_.at(x, y) &&
                   costs_.at(current.x, current.y) == costs_.at(x, y) + 1001) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
        x = current.x + 2;
        y = current.y;
        if (coming_from_.at(current.x, current.y) ==
                static_cast<int>(Direction::EAST) &&
            coming_from_.at(x, y) == static_cast<int>(Direction::EAST) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 2) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x - 1, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
      }

      // South
      x = current.x;
      y = current.y + 1;
      if (maze_.get_tile_at(x, y) != Tile::Wall) {
        if (coming_from_.at(current.x, current.y) == coming_from_.at(x, y) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 1) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        } else if (coming_from_.at(current.x, current.y) !=
                       coming_from_.at(x, y) &&
                   costs_.at(current.x, current.y) == costs_.at(x, y) + 1001) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
        x = current.x;
        y = current.y + 2;
        if (coming_from_.at(current.x, current.y) ==
                static_cast<int>(Direction::SOUTH) &&
            coming_from_.at(x, y) == static_cast<int>(Direction::SOUTH) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 2) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y - 1, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
      }

      // West
      x = current.x - 1;
      y = current.y;
      if (maze_.get_tile_at(x, y) != Tile::Wall) {
        if (coming_from_.at(current.x, current.y) == coming_from_.at(x, y) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 1) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        } else if (coming_from_.at(current.x, current.y) !=
                       coming_from_.at(x, y) &&
                   costs_.at(current.x, current.y) == costs_.at(x, y) + 1001) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
        x = current.x - 2;
        y = current.y;
        if (coming_from_.at(current.x, current.y) ==
                static_cast<int>(Direction::WEST) &&
            coming_from_.at(x, y) == static_cast<int>(Direction::WEST) &&
            costs_.at(current.x, current.y) == costs_.at(x, y) + 2) {
          queue.push_back({x, y, Direction::NONE});
          optimal_seating_spots.insert({x + 1, y, Direction::NONE});
          optimal_seating_spots.insert({x, y, Direction::NONE});
        }
      }
    }
    return optimal_seating_spots;
  }

private:
  const Location start_;
  const Location end_;
  const TileArray maze_;
  array2D::Array2D<int> costs_;
  array2D::Array2D<int> coming_from_;
  int path_length_;
};

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
  DijkstraPathing dijkstra_pathing =
      DijkstraPathing(reindeer_maze, {start_x, start_y, Direction::EAST},
                      {end_x, end_y, Direction::NONE});
  int total_score = dijkstra_pathing.get_path_length();
  std::cout << "Total score: " << total_score << "\n";

  // Task 2 : Optimal seating spot
  auto optimal_seating_spots = dijkstra_pathing.get_optimal_seating_spots();
  std::cout << "Optimal seating spots: " << optimal_seating_spots.size()
            << "\n";

  array2D::Array2D<char> visualize_seating_spots(reindeer_maze.length(),
                                                 reindeer_maze.height(), ' ');
  std::ranges::for_each(optimal_seating_spots, [&](const auto &seating_spot) {
    visualize_seating_spots.at(seating_spot.x, seating_spot.y) = '0';
  });
  std::function<std::string(char)> char_id = [](auto v) -> std::string {
    return std::string(1, v);
  };
  for(std::size_t x = 0; x < reindeer_maze.length(); ++x) {
    for(std::size_t y = 0; y < reindeer_maze.height(); ++y) {
      if(reindeer_maze.get_tile_at(x, y) == Tile::Wall){
        visualize_seating_spots.at(x, y) = '#';
      }
    }
  }
  array2D::print_array(visualize_seating_spots, char_id);
  /*
  std::function<std::string(int)> func_int = [](int val) -> std::string {
    int width = 7;
    if (val == INT_MAX - 2000) {
      return std::string(width, '#');
    }
    std::string ret = " " + std::to_string(val);
    return ret + std::string(width - ret.size(), ' ');
  };
  array2D::print_array(dijkstra_pathing.get_costs(), func_int);
  */
  /*
  std::function<std::string(int)> func_direction = [](int direction) ->
  std::string { switch (direction) { case static_cast<int>(Direction::NONE):
  return "#"; case static_cast<int>(Direction::NORTH): return "^"; case
  static_cast<int>(Direction::EAST): return ">"; case
  static_cast<int>(Direction::SOUTH): return "v"; case
  static_cast<int>(Direction::WEST): return "<";
    }
    return "#";
  };
  array2D::print_array(dijkstra_pathing.get_coming_from(), func_direction);
  */
  
}