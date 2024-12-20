#include <cstddef>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>


struct Towels {
  std::vector<std::string> patterns{};
  std::vector<std::string> designs{};
};

Towels read_input(std::string file_location) {
  std::fstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::string line;
  Towels towels{};
  std::getline(input_file, line);
  std::size_t towel_length{}, idx = 0;
  while (idx < line.size()) {
    towel_length = line.substr(idx).find(",");
    towels.patterns.push_back(line.substr(idx, towel_length));
    idx += towel_length + 2;
  }
  // Last element incorrectly added due to find returning npos
  towels.patterns.erase(--towels.patterns.end());
  std::getline(input_file, line);
  while (std::getline(input_file, line)) {
    towels.designs.push_back(line);
  }
  return towels;
}

std::vector<std::string> get_possible_designs(const Towels &towels) {
  std::string regex_string = "(";
  for (int i = 0; i < towels.patterns.size() - 1; ++i) {
    regex_string += towels.patterns.at(i) + "|";
  }
  regex_string += towels.patterns.at(towels.patterns.size() - 1);
  regex_string += ")+";

  std::regex possible_patterns(regex_string);
  std::smatch pattern_match;

  std::vector<std::string> possible_designs{};
  for (const auto &design : towels.designs) {
    if (std::regex_match(design, pattern_match, possible_patterns)) {
      possible_designs.push_back(design);
    }
  }

  return possible_designs;
}

int main(int argc, char **argv) {
  std::ifstream input_file;
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // Read input
  Towels towels = read_input(argv[1]);

  // Task 1
  int sum_of_possible_designs = get_possible_designs(towels).size(); // TODO
  std::cout << "Possible designs: " << sum_of_possible_designs << "\n";

  // Task 2
}