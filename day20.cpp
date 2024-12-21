#include <cstdlib>
#include <iostream>

#include <array2d.h>
#include <shortest_path.h>
#include <vector>

enum Tile : int {
  WALL = paths::Dijkstra::kWALL,
  AIR = paths::Dijkstra::kAIR,
  START = 2,
  END = 3
};

int distance(paths::Point from, paths::Point to) {
  return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}

int get_path_length_with_cheating(paths::Point cheating_start,
                                  paths::Point cheating_end,
                                  int path_length_without_cheating,
                                  const paths::Dijkstra &pathfinding) {
  int dist_to_begin = pathfinding.get_shortest_path_length(cheating_start);
  int dist_to_end = pathfinding.get_shortest_path_length(cheating_end);
  return path_length_without_cheating -
         (dist_to_end -
          (dist_to_begin + distance(cheating_start, cheating_end)));
}

std::vector<paths::Point> get_cheats_from(paths::Point p,
                                          const array2D::Array2D<int>& maze) {
  static const std::vector<paths::Point> cheat_directions{
      {2, 0}, {-2, 0}, {0, 2}, {0, -2}};

  std::vector<paths::Point> cheats{};
  for (const auto &cheat_direction : cheat_directions) {
    paths::Point cheat_end = p + cheat_direction;
    if ((0 <= cheat_end.x && cheat_end.x < maze.length()) &&
        (0 <= cheat_end.y && cheat_end.y < maze.height())) {
      paths::Point halfway = p + (cheat_direction / 2);
      if (maze.cat(cheat_end.x, cheat_end.y) != paths::Dijkstra::kWALL &&
          maze.cat(halfway.x, halfway.y) == paths::Dijkstra::kWALL) {
        cheats.push_back(cheat_end);
      }
    }
  }
  return cheats;
};

int get_sum_of_cheats_with_min_timesave(int min_timesave,
                                        const array2D::Array2D<int> &maze,
                                        paths::Point start, paths::Point end) {
  paths::Dijkstra pathfinding{maze, start};
  const auto path = pathfinding.get_shortest_path(end);
  int path_length = path.size() - 1;

  int sum_of_cheats = 0;
  for (const auto &p : path) {
    auto cheats = get_cheats_from(p, maze);
    for (auto cheat : cheats) {
      int path_length_with_cheat =
          get_path_length_with_cheating(p, cheat, path_length, pathfinding);
      if (path_length - path_length_with_cheat >= min_timesave ) {
        sum_of_cheats++;
      }
    }
  }
  return sum_of_cheats;
}

// Task 2
std::vector<paths::Point> get_cheats(const std::vector<paths::Point>& path, int idx, int max_distance) {
  std::vector<paths::Point> cheats {};
  for(int idx2 = idx + 1; idx2 < path.size(); ++idx2){
    if(distance(path.at(idx), path.at(idx2)) <= max_distance){
      cheats.push_back(path.at(idx2));
    }
  }
  return cheats;
}

int get_sum_of_cheats(int cheat_distance, int min_timesave,
                      const array2D::Array2D<int> &maze,
                      paths::Point start, paths::Point end) {
  paths::Dijkstra pathfinding{maze, start};
  const auto path = pathfinding.get_shortest_path(end);
  int path_length = path.size() - 1;
  
  int sum_of_cheats = 0;
  for (int i = 0; i < path.size(); ++i) {
    auto cheats = get_cheats(path, i, cheat_distance);
    for (auto cheat : cheats) {
      int path_length_with_cheat =
          get_path_length_with_cheating(path.at(i), cheat, path_length, pathfinding);
      if (path_length - path_length_with_cheat >= min_timesave ) {
        sum_of_cheats++;
      }
    }
  }
  return sum_of_cheats;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  std::function<int(char)> char_to_int = [](char c) -> int {
    if (c == '#') {
      return Tile::WALL;
    } else if (c == 'S') {
      return Tile::START;
    } else if (c == 'E') {
      return Tile::END;
    }
    return Tile::AIR;
  };
  std::function<std::string(int)> int_to_string = [](int i) -> std::string {
    switch (i) {
    case Tile::WALL:
      return "#";
    case Tile::AIR:
      return " ";
    case Tile::START:
      return "E";
    }
    return "S";
  };
  // Read input
  paths::Point start{0, 0}, end{0, 0};
  array2D::Array2D<int> maze = array2D::read_array(argv[1], char_to_int);
  for (int x = 0; x < maze.length(); ++x) {
    for (int y = 0; y < maze.height(); ++y) {
      if (maze.cat(x, y) == Tile::START) {
        start = {x, y};
      }
      if (maze.cat(x, y) == Tile::END) {
        end = {x, y};
      }
    }
  }

  
  paths::Dijkstra pathfinding{maze, start};
  auto path = pathfinding.get_shortest_path(end);
  int path_length_without_cheating = path.size() - 1;

  std::cout << "Length of shortest path without cheating: "
            << path_length_without_cheating << "\n";

  int sum_of_cheats_with_timesave_2 = get_sum_of_cheats_with_min_timesave(2, maze, start, end);
  std::cout << "Sum of cheats with timesave >= 2: " << sum_of_cheats_with_timesave_2 << "\n";

  int sum_of_cheats_with_timesave_10 = get_sum_of_cheats_with_min_timesave(10, maze, start, end);
  std::cout << "Sum of cheats with timesave >= 10: " << sum_of_cheats_with_timesave_10 << "\n";

  int sum_of_cheats_with_timesave_100 = get_sum_of_cheats_with_min_timesave(100, maze, start, end);
  std::cout << "Sum of cheats with timesave >= 100: " << sum_of_cheats_with_timesave_100 << "\n";

  int sum_of_cheats_with_timesave_100_and_distance_20 = get_sum_of_cheats(20, 100, maze, start, end);
  std::cout << "Sum of cheats with timesave >= 100 and distance <= 20: " << sum_of_cheats_with_timesave_100_and_distance_20 << "\n";
  // array2D::print_array(maze, int_to_string);
}