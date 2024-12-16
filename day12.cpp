#include <algorithm>
#include <cstddef>
#include <deque>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <string>

#include "array2d.h"
#include "vector.h"

using Farm = array2D::Array2D<char>;

template <class T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &set) {
  os << "[";
  std::ranges::for_each(set, [&](const auto &val) { os << val << ", "; });
  os << "]";
  return os;
}

// ===== Task 1 =====
using Region = std::set<vector::Vector>;


Region get_region(vector::Vector start, const Farm &farm) {
  std::deque<vector::Vector> queue{start};
  Region region{};
  const char plant = farm.cat(start.x, start.y);
  while (!queue.empty()) {
    vector::Vector current = queue.back();
    queue.pop_back();

    // Is plot correct plant type?
    if (farm.cat(current.x, current.y) != plant) {
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

int calculate_circumference(const Region &region) {
  int circumference = 4 * region.size();
  for (const auto &plot : region) {
    if (plot.x > 0 && region.contains({plot.x - 1, plot.y})) {
      --circumference;
    }
    if (region.contains({plot.x + 1, plot.y})) {
      --circumference;
    }
    if (plot.y > 0 && region.contains({plot.x, plot.y - 1})) {
      --circumference;
    }
    if (region.contains({plot.x, plot.y + 1})) {
      --circumference;
    }
  }
  return circumference;
}

int get_cost_of_fences(const Farm &farm) {
  int sum = 0;
  array2D::Array2D<int> is_in_region{farm.length(), farm.height(), 0};
  for (int y = 0; y < farm.height(); ++y) {
    for (int x = 0; x < farm.length(); ++x) {
      if (is_in_region.at(x, y) == 0) {
        Region region = get_region({x, y}, farm);
        std::ranges::for_each(region, [&](const auto &val) {
          is_in_region.at(val.x, val.y) = 1;
        });
        int area = region.size();
        int circumference = calculate_circumference(region);
        sum += area * circumference;
      }
    }
  }
  return sum;
}

// ===== Task 2 =====
int calculate_sides(const Region& region){
    // for each side of each point, track if it is part of an already calculate side
    // if it is in a side at all
    // UP, RIGHT, DOWN, LEFT
    const int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;
    std::map<vector::Vector, std::array<bool, 4>> is_in_side;
    std::ranges::for_each(region, [&](const auto& val){is_in_side[val] = {false, false, false, false};});
    
    int sides = 0;
    for (const auto& point : region) {
        // UP ( y - 1)
        if(!region.contains({point.x, point.y - 1}) && !is_in_side[point][UP]) {
            ++sides;

            // Mark side;
            is_in_side[point][UP] = true;
            // Check right
            int offset = 1;
            vector::Vector offset_point {point.x + offset, point.y }, offset_adjacent_point {point.x + offset, point.y - 1};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][UP] = true;
                ++offset;
                offset_point = {point.x + offset, point.y };
                offset_adjacent_point = {point.x + offset, point.y - 1};
            }
            // Check left
            offset = -1;
            offset_point = {point.x + offset, point.y };
            offset_adjacent_point = {point.x + offset, point.y - 1};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][UP] = true;
                --offset;
                offset_point = {point.x + offset, point.y };
                offset_adjacent_point = {point.x + offset, point.y - 1};
            }
        }


        // DOWN ( y + 1)
        if(!region.contains({point.x, point.y + 1}) && !is_in_side[point][DOWN]) {
            ++sides;

            // Mark side;
            is_in_side[point][DOWN] = true;
            // Check right
            int offset = 1;
            vector::Vector offset_point {point.x + offset, point.y }, offset_adjacent_point {point.x + offset, point.y + 1};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][DOWN] = true;
                ++offset;
                offset_point = {point.x + offset, point.y };
                offset_adjacent_point = {point.x + offset, point.y + 1};
            }
            // Check left
            offset = -1;
            offset_point = {point.x + offset, point.y };
            offset_adjacent_point = {point.x + offset, point.y - 1};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][DOWN] = true;
                --offset;
                offset_point = {point.x + offset, point.y };
                offset_adjacent_point = {point.x + offset, point.y - 1};
            }
        }

        // RIGHT ( x + 1)
        if(!region.contains({point.x + 1, point.y}) && !is_in_side[point][RIGHT]) {
            ++sides;

            // Mark side;
            is_in_side[point][RIGHT] = true;
            // Check down
            int offset = 1;
            vector::Vector offset_point {point.x , point.y + offset }, offset_adjacent_point {point.x + 1, point.y + offset};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][RIGHT] = true;
                ++offset;
                offset_point = {point.x , point.y + offset};
                offset_adjacent_point = {point.x + 1, point.y + offset};
            }
            // Check up
            offset = -1;
            offset_point = {point.x, point.y + offset };
            offset_adjacent_point = {point.x + 1, point.y + offset};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][RIGHT] = true;
                --offset;
                offset_point = {point.x, point.y + offset};
                offset_adjacent_point = {point.x + 1, point.y + offset};
            }
        }


        // LEFT ( x - 1)
        if(!region.contains({point.x - 1, point.y}) && !is_in_side[point][LEFT]) {
            ++sides;

            // Mark side;
            is_in_side[point][LEFT] = true;
            // Check down
            int offset = 1;
            vector::Vector offset_point {point.x , point.y + offset }, offset_adjacent_point {point.x - 1, point.y + offset};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][LEFT] = true;
                ++offset;
                offset_point = {point.x , point.y + offset};
                offset_adjacent_point = {point.x - 1, point.y + offset};
            }
            // Check up
            offset = -1;
            offset_point = {point.x, point.y + offset };
            offset_adjacent_point = {point.x - 1, point.y + offset};
            while(region.contains(offset_point) && !region.contains(offset_adjacent_point)) {
                is_in_side[offset_point][LEFT] = true;
                --offset;
                offset_point = {point.x, point.y + offset};
                offset_adjacent_point = {point.x - 1, point.y + offset};
            }
        }
    }
    return sides;
}

int get_cost_of_fences_with_bulk_discount(const Farm &farm) {
  int sum = 0;
  array2D::Array2D<int> is_in_region{farm.length(), farm.height(), 0};
  for (std::size_t y = 0; y < farm.height(); ++y) {
    for (std::size_t x = 0; x < farm.length(); ++x) {
      if (is_in_region.at(x, y) == 0) {
        Region region = get_region({static_cast<int>(x), static_cast<int>(y)}, farm);
        std::ranges::for_each(region, [&](const auto &val) {
          is_in_region.at(val.x, val.y) = 1;
        });
        int area = region.size();
        int sides = calculate_sides(region);
        //std::cout << region << " | Char: " << farm.at(region.begin()->x, region.begin()->y) << " | Size: " << area << " | Sides: " << sides << "\n";
        sum += area * sides;
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
  Farm farm = array2D::read_array<char>(argv[1], [](char c){return c;});
  // Farm farm = read_input(argv[1]);

  // Task 1 : Correctly ordered pages
  int cost_of_fences = get_cost_of_fences(farm);
  std::cout << "Cost of fences: " << cost_of_fences << "\n";
  // Task 2 :
  int cost_of_fences_with_bulk_discount = get_cost_of_fences_with_bulk_discount(farm);
  std::cout << "Cost of fences with bulk discount: " << cost_of_fences_with_bulk_discount << "\n";
}