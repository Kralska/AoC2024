#include <algorithm>
#include <cstddef>
#include <deque>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

template <class T> class Array2D {
public:
  Array2D(std::size_t length, std::size_t height, T default_value = {})
      : length_(length), height_(height) {
    // Initialize empty, '0' doesn't appear
    vec_ = std::vector<T>(length_ * height_, default_value);
  }
  Array2D(std::size_t length, std::size_t height,
          std::ranges::input_range auto &&range)
      : Array2D(length, height) {
    int idx = 0;
    std::ranges::for_each(range, [&](auto &val) {
      vec_.at(idx) = val;
      ++idx;
    });
  }

  T &at(std::size_t x, std::size_t y) { return vec_.at(y * length_ + x); }
  const char &at(int x, int y) const { return vec_[y * length_ + x]; }
  const std::size_t &height() const { return height_; }
  const std::size_t &length() const { return length_; }

private:
  std::vector<T> vec_;
  const std::size_t length_;
  const std::size_t height_;
};

using Farm = Array2D<char>;
struct Point {
  std::size_t x, y;
  Point(std::size_t x, std::size_t y) : x(x), y(y) {}

  auto operator<=>(const Point &) const = default;
};
std::ostream &operator<<(std::ostream &os, Point p) {
  os << "(" << p.x << ", " << p.y << ")";
  return os;
}

void print_farm(const Farm &farm) {
  for (int y = 0; y < farm.height(); ++y) {
    for (int x = 0; x < farm.length(); ++x) {
      std::cout << farm.at(x, y);
    }
    std::cout << "\n";
  }
}

Farm read_input(std::string file_location) {
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::string farm_str = "";
  std::string line;
  int length = 0, height = 0;
  while (std::getline(input_file, line)) {
    ++height;
    length = line.size();
    farm_str += line;
  }
  Farm farm(length, height, farm_str);
  return farm;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &set) {
  os << "[";
  std::ranges::for_each(set, [&](const auto &val) { os << val << ", "; });
  os << "]";
  return os;
}

// ===== Task 1 =====
using Region = std::set<Point>;
Region get_region(Point start, const Farm &farm) {
  std::deque<Point> queue{start};
  Region region{};
  const char plant = farm.at(start.x, start.y);
  while (!queue.empty()) {
    Point current = queue.back();
    queue.pop_back();

    // Is plot correct plant type?
    if (farm.at(current.x, current.y) != plant) {
      continue;
    }
    // Plot already in region?
    if (region.contains(current)) {
      continue;
    }

    region.insert(current);
    // Queue neighbors
    if (current.y + 1 < farm.height()) {
      queue.push_back({current.x, current.y + 1});
    }
    if (current.y > 0) {
      queue.push_back({current.x, current.y - 1});
    }
    if (current.x + 1 < farm.height()) {
      queue.push_back({current.x + 1, current.y});
    }
    if (current.x > 0) {
      queue.push_back({current.x - 1, current.y});
    }
  }
  return region;
}

int calculate_circumference(const Region &region, const Farm &farm) {
  int circumference = 4 * region.size();
  for (const auto &plot : region) {
    if (plot.x > 0 && region.contains({plot.x - 1, plot.y})) {
      --circumference;
    }
    if (plot.x + 1 < farm.length() && region.contains({plot.x + 1, plot.y})) {
      --circumference;
    }
    if (plot.y > 0 && region.contains({plot.x, plot.y - 1})) {
      --circumference;
    }
    if (plot.y + 1 < farm.length() && region.contains({plot.x, plot.y + 1})) {
      --circumference;
    }
  }
  return circumference;
}

int get_cost_of_fences(const Farm &farm) {
  int sum = 0;
  Array2D<int> is_in_region{farm.length(), farm.height(), 0};
  for (std::size_t y = 0; y < farm.height(); ++y) {
    for (std::size_t x = 0; x < farm.length(); ++x) {
      if (is_in_region.at(x, y) == 0) {
        Region region = get_region({x, y}, farm);
        std::ranges::for_each(region, [&](const auto &val) {
          is_in_region.at(val.x, val.y) = 1;
        });
        int area = region.size();
        int circumference = calculate_circumference(region, farm);
        sum += area * circumference;
      }
    }
  }
  return sum;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  Array2D farm = read_input(argv[1]);

  // Task 1 : Correctly ordered pages
  int cost_of_fences = get_cost_of_fences(farm);
  std::cout << "Cost of fences: " << cost_of_fences << "\n";
  // Task 2 :
}