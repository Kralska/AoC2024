#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

template <class Key, class Value>
std::ostream& operator<<(std::ostream& os, const std::map<Key, Value>& map) {
    os << "{";
    for(const auto& pair : map){
        os << "[" << pair.first << ": " << pair.second << "], ";
    }
    os << "}";
    return os;
}

using Stone = unsigned long long;

int digits(const Stone stone) {
    return std::to_string(stone).size();
}

std::vector<Stone> blink(Stone stone){
    if (stone == 0) {
        return {1};
    }
    if (digits(stone) % 2 == 0){
        auto as_string = std::to_string(stone);
        std::vector<Stone> stones {};
        stones.push_back(std::stoull(as_string.substr(0, as_string.size() / 2)));
        stones.push_back(std::stoull(as_string.substr(as_string.size() / 2)));
        return stones;
    }
    return {2024 * stone};
}

std::map<Stone, unsigned long long> blink(const std::map<Stone, unsigned long long>& map){
    std::map<Stone, unsigned long long> blinked {};
    for(const auto& pair : map){
        auto blinked_stone = blink(pair.first);
        for(const auto& stone : blinked_stone) {
            blinked[stone] += pair.second;
        }
    }
    return blinked;
}

std::map<Stone, unsigned long long> blink(std::map<Stone, unsigned long long> map, unsigned int times){
  for(int i = 0; i < times; ++i){
    map = blink(map);
  }
  return map;
}

unsigned long long total_stones(const std::map<Stone, unsigned long long>& map) {
    unsigned long  long sum = 0;
    std::ranges::for_each(map, [&](const auto& pair){sum += pair.second;});
    return sum;
}

int main(int argc, char **argv) {
  std::ifstream input_file;
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  input_file.open(argv[1]);
  if (!input_file.is_open()) {
    std::cerr << "Unable to open file." << std::endl;
    return 0;
  }

  // Read input
  std::vector<unsigned long long> initial_stones{};
  std::string line;
  if(!std::getline(input_file, line)){
    std::cerr << "Expected at least one line of text" << "\n";
  }
  std::size_t read_chars {};
  std::size_t idx = 0;
  while(idx < line.size()){
    unsigned long long stone = std::stoull(line.substr(idx), &read_chars);
    initial_stones.push_back(stone);
    idx += read_chars;
  }
  std::map<Stone, unsigned long long> stone_to_amount {};
  std::ranges::for_each(initial_stones, [&](const auto& val){stone_to_amount[val]++;});

  
  // Task 1
  std::cout << "Size after " << 25 <<" blinks: " << total_stones(blink(stone_to_amount, 25)) << "\n";

  // Task 2
  std::cout << "Size after " << 75 <<" blinks: " << total_stones(blink(stone_to_amount, 75)) << "\n";
}