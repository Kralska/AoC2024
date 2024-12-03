#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int calculate_total_distance(std::vector<int> left, std::vector<int> right) {
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  int sum = 0;
  for (int idx = 0; idx < left.size(); idx++) {
    sum += std::abs(left[idx] - right[idx]);
  }
  return sum;
}

int calculate_similarity(std::vector<int> left, std::vector<int> right) {
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  int sum = 0;
  for (int number : left) {
    sum += number * std::count(right.begin(), right.end(), number);
  }
  return sum;
}

int main(int argc, char **argv) {
  std::ifstream input_file;
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
  }

  // First argument is input file location
  input_file.open(argv[1]);
  if (!input_file.is_open()) {
    std::cerr << "Unable to open file." << std::endl;
    return (0);
  }

  // Read input
  std::string line;
  std::vector<int> left{}, right{};
  while (std::getline(input_file, line)) {
    std::size_t idx{};
    int newLeft = std::stoi(line, &idx);
    int newRight = std::stoi(line.substr(idx), &idx);

    left.push_back(newLeft);
    right.push_back(newRight);
  }

  assert(left.size() == right.size());

  int total_distance = calculate_total_distance(left, right);
  std::cout << "Total distance between lists: " << total_distance << "\n";

  int similarity = calculate_similarity(left, right);
  std::cout << "Similarity score: " << similarity << "\n";
}
