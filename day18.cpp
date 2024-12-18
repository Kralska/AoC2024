#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <vector>


#include "array2d.h"
#include "shortest_path.h"

std::vector<paths::Point> read_input(std::string file_location) {
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::vector<paths::Point> coordinates{};
  std::string line;
  std::size_t read_chars{};
  while (std::getline(input_file, line)) {
    int x = std::stoi(line, &read_chars);
    int y = std::stoi(line.substr(read_chars + 1));

    coordinates.push_back({x, y});
  }
  return coordinates;
}

int get_shortest_path(std::size_t width, std::size_t height, paths::Point from,
                      paths::Point to, std::vector<paths::Point> obstacles,
                      int num_obstacles) {
  array2D::Array2D<int> memory{width, height, paths::Dijkstra::kAIR};
  auto view = obstacles | std::views::take(num_obstacles);
  std::ranges::for_each(view, [&](const auto &p) {
    memory.at(p.x, p.y) = paths::Dijkstra::kWALL;
  });

  paths::Dijkstra dijkstra{memory, from};
  return dijkstra.get_shortest_path_length(to);
}

paths::Point get_first_disabling_byte(std::size_t width, std::size_t height,
                                      paths::Point from, paths::Point to,
                                      std::vector<paths::Point> obstacles) {
  std::size_t l = 0;
  std::size_t r = obstacles.size();
  while (r - l > 2) { // End condition
    std::size_t m = (l + r) / 2;
    if (get_shortest_path(width, height, from, to, obstacles, m) < INT_MAX) {
      l = m + 1;
    } else {
      r = m;
    }
    //std::cout << "l: " << l << "| r: " << r << "\n";
  }
  if (get_shortest_path(width, height, from, to, obstacles, l) == INT_MAX) {
    return obstacles.at(l - 1);
  } else if (get_shortest_path(width, height, from, to, obstacles, l + 1) ==
             INT_MAX) {
    return obstacles.at(l);
  } else if (get_shortest_path(width, height, from, to, obstacles, l + 2) ==
             INT_MAX) {
    return obstacles.at(l + 1);
  }
  return obstacles.at(l + 2);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  std::vector<paths::Point> byte_coordinates = read_input(argv[1]);
  array2D::Array2D<int> memory{71, 71, paths::Dijkstra::kAIR};
  auto view = byte_coordinates | std::views::take(1024);
  std::ranges::for_each(view, [&](const auto &p) {
    memory.at(p.x, p.y) = paths::Dijkstra::kWALL;
  });
  paths::Dijkstra dijkstra{memory, {0, 0}};

  std::function<std::string(int)> func = [](int val) {
    if (val == 2) {
      return "O";
    }
    if (val == 1) {
      return " ";
    }
    return "#";
  };
  // std::ranges::for_each(path, [&](const auto &p){memory.at(p.x, p.y) = 2;});
  //array2D::print_array(memory, func);
  //std::cout << "\n";

  const std::size_t width = 71; // 7
  const std::size_t height = 71; // 7
  paths::Point from {0, 0};
  paths::Point to {70, 70}; //{6, 6};
  const int num_obstacles = 1024; //12;

  int shortest_path_length =
      get_shortest_path(width, width, from, to, byte_coordinates, num_obstacles);
  std::cout << "Shortest path: " << shortest_path_length << "\n";

  paths::Point first_disabling_byte =
      get_first_disabling_byte(width, height, from, to, byte_coordinates);
  std::cout << "First disabling byte: " << first_disabling_byte.x << "," << first_disabling_byte.y;
}