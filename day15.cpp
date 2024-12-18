#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>

#include "array2d.h"

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

enum class Tile : int { Empty = 0, Wall = 1, Box = 2, Robot = 3 };

class TileArray : public array2D::Array2D<int> {
public:
  TileArray(std::size_t width, std::size_t height,
            Tile defaultTile = Tile::Empty)
      : array2D::Array2D<int>(width, height, static_cast<int>(defaultTile)) {}

  TileArray(const array2D::Array2D<int> &arr) : array2D::Array2D<int>(arr) {
    bool found = false;
    for (int x = 0; x < length(); ++x) {
      for (int y = 0; y < height(); ++y) {
        if (get_tile_at(x, y) == Tile::Robot) {
          robot_x = x;
          robot_y = y;
          found = true;
          break;
        }
      }
      if (found) {
        break;
      }
    }
  }

  Tile get_tile_at(std::size_t x, std::size_t y) const {
    auto at_pos = cat(x, y);
    assert((0 <= at_pos && at_pos <= 3));
    return static_cast<Tile>(at_pos);
  }

  void set_tile_at(std::size_t x, std::size_t y, Tile tile) {
    at(x, y) = static_cast<int>(tile);
    if (tile == Tile::Robot) {
      robot_x = x;
      robot_y = y;
    }
  }

  void move(char direction) {
    int dir_x = 0, dir_y = 0;
    if (direction == '^') {
      dir_y = -1;
    } else if (direction == '>') {
      dir_x = 1;
    } else if (direction == 'v') {
      dir_y = 1;
    } else if (direction == '<') {
      dir_x = -1;
    }

    int i = 1;
    std::size_t start_x = robot_x, start_y = robot_y;
    Tile tile = get_tile_at(start_x + i * dir_x, start_y + i * dir_y);
    while (tile != Tile::Wall && tile != Tile::Empty) {
      ++i;
      tile = get_tile_at(start_x + i * dir_x, start_y + i * dir_y);
    }
    if (tile == Tile::Empty) {
      for (; i > 0; --i) {
        set_tile_at(
            robot_x + i * dir_x, robot_y + i * dir_y,
            get_tile_at(robot_x + (i - 1) * dir_x, robot_y + (i - 1) * dir_y));
      }
      set_tile_at(start_x, start_y, Tile::Empty);
    }
  }

  void move(std::string movements) {
    for (const auto &c : movements) {
      move(c);
    }
  }

  int calculate_gps_score(std::size_t x, std::size_t y) const {
    if (get_tile_at(x, y) == Tile::Box) {
      return 100 * y + x;
    }
    return 0;
  }

  int calculate_total_gps_score() const {
    int sum = 0;
    for (int x = 0; x < length(); ++x) {
      for (int y = 0; y < height(); ++y) {
        sum += calculate_gps_score(x, y);
      }
    }
    return sum;
  }

private:
  std::size_t robot_x = 0, robot_y = 0;
};

void print(const TileArray &warehouse) {
  std::function<std::string(int)> to_char = [](const int &v) -> std::string {
    const static std::map<int, std::string> tile_to_char{
        {static_cast<int>(Tile::Wall), "#"},
        {static_cast<int>(Tile::Robot), "@"},
        {static_cast<int>(Tile::Box), "O"},
        {static_cast<int>(Tile::Empty), " "},
    };
    return tile_to_char.at(v);
  };
  array2D::print_array(warehouse, to_char);
}

// ===== Read Input =====
TileArray read_warehouse(std::string file_location) {
  std::function<int(char)> ctoi = [](char c) {
    const static std::map<char, Tile> char_to_tile{
        {'#', Tile::Wall},
        {'.', Tile::Empty},
        {'@', Tile::Robot},
        {'O', Tile::Box},
    };
    return static_cast<int>(char_to_tile.at(c));
  };
  array2D::Array2D<int> array = array2D::read_array(file_location, ctoi);
  return TileArray(array);
}

std::string read_directions(std::string file_location) {
  std::string directions = "";

  std::string line;
  std::ifstream input_file;
  input_file.open(file_location);
  // Advance to directions part
  while (std::getline(input_file, line) && line != "") {
  }

  while (std::getline(input_file, line)) {
    directions += line;
  }
  return directions;
}

int get_sum_of_gps_coordinates(const TileArray &warehouse) { return 0; }

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  TileArray warehouse = read_warehouse(argv[1]);
  std::string directions = read_directions(argv[1]);
  // Task 1 : Sum of gps coordinates
  warehouse.move(directions);
  int total_gps_score = warehouse.calculate_total_gps_score();
  std::cout << "Sum of gps coordinates: " << total_gps_score << "\n";
}