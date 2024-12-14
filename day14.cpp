#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>

#include "vector.h"
#include "array2d.h"

using Vector = vector::Vector;

struct Robot {
  Vector start;
  Vector velocity;

  Robot(Vector start, Vector velocity) : start(start), velocity(velocity) { }
};

std::ostream& operator<<(std::ostream& os, const Robot& robot) {
  os << "[Start: " << robot.start << "| Velocity: " << robot.velocity << "]";
  return os;
}

std::vector<Robot> read_input(std::string file_location) {
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::string line;
  std::size_t offset {};
  std::vector<Robot> robots {};
  while (std::getline(input_file, line)) {
    int start_x = std::stoi(line.substr(line.find("p=") + 2), &offset);
    int start_y = std::stoi(line.substr(line.find("p=") + 3 + offset)); 
  
    int velocity_x = std::stoi(line.substr(line.find("v=") + 2), &offset);
    int velocity_y = std::stoi(line.substr(line.find("v=") + 3 + offset));

    robots.push_back({{start_x, start_y}, {velocity_x, velocity_y}});
  }
  return robots;
}

Vector get_robot_position(const Robot& robot, int moves = 0, const int width = 101, const int height = 103) {
  Vector pos = robot.start;
  for(int i = 0; i < moves; ++i){
    pos += robot.velocity;
    pos.x = pos.x % width;
    if(pos.x < 0) {
      pos.x += width;
    }
    pos.y = pos.y % height;
    if(pos.y < 0) {
      pos.y += height;
    }
  }
  return pos;
}

// ===== Task 2 =====
int get_safety_factor(const std::vector<Robot>& robots){
  const std::size_t width = 101, height = 103;
  
  std::vector<Vector> final_positions {};
  for(const auto& robot : robots) {
    final_positions.push_back(get_robot_position(robot, 100, width, height));
  }

  int upper_left = 0, upper_right = 0, lower_left = 0, lower_right = 0;
  for(const auto& position : final_positions) {
    if(position.x < width / 2){
      if (position.y < height / 2) {
        ++upper_left;
      } else if (position.y > height / 2) {
        ++lower_left;
      }
    } else if (position.x > width / 2) {
      if (position.y < height / 2) {
        ++upper_right;
      } else if (position.y > height / 2) {
        ++lower_right;
      }
    }
  }
  return upper_left * upper_right * lower_left * lower_right;
}

void display_robots(const std::vector<Robot>& robots, int moves = 0){
  const std::size_t width = 101, height = 103;
  
  std::vector<Vector> final_positions {};
  for(const auto& robot : robots) {
    final_positions.push_back(get_robot_position(robot, moves, width, height));
  }
  
  Array2D::Array2D<char> array {width, height, ' '};
  for(const auto& pos : final_positions) {
    array.at(pos.x, pos.y) = 'X';
  }
  Array2D::print_array(array);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  std::vector<Robot> robots = read_input(argv[1]);

  // Task 1 : Safety factor
  int safety_factor = get_safety_factor(robots);
  std::cout << "Safety factor: " << safety_factor << "\n";
  // Task 2 : Tree easter egg
  char c = 'y';
  int i = 79;
  while (c != 'n'){
    std::cout << "i = " << i << "\n";
    display_robots(robots,i);
    i += 101;
    std::cout << "\n";
    std::cout << "Continue?(y/n)" << "\n";
    std::cin >> c;
  }

}