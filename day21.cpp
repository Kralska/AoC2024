
#include <fstream>
#include <iostream>
#include <map>
#include <sec_api/stdio_s.h>
#include <string>
#include <utility>
#include <vector>

// ===== INPUT =====
std::vector<std::string> read_input(std::string file_location) {
  std::fstream input_file;
  input_file.open(file_location);
  if(!input_file.is_open()){
    throw std::invalid_argument("Unable to open file at " + file_location);
  }
 
  std::vector<std::string> sequences {};
  std::string line;
  while(std::getline(input_file, line)) {
    sequences.push_back(line);
  }
  return sequences; 
}

// ===== NUMERICAL KEYPAD =====
/**
 * 789
 * 456
 * 123
 * X0A
 */

class NumericalKeypad {
public:

  NumericalKeypad(char start) : pos(start) { }; 

  static std::string get_sequence(char from, char to) {
    auto from2dpos = char_to_2dpos.at(from);
    auto to2dpos = char_to_2dpos.at(to);

    int move_x = to2dpos.first - from2dpos.first;
    int move_y = to2dpos.second - from2dpos.second;

    std::string res = "";
    if(move_y < 0){
        res += std::string(-move_y, '^');
    }
    if(move_x < 0){
        res += std::string(-move_x, '<');
    }
    if(move_x >= 0){
        res += std::string(move_x, '>');
    }
    if(move_y >= 0){
        res += std::string(move_y, 'v');
    }
    return res;
  }

  std::string move_to(char to){
    char old_pos = pos;
    pos = to;
    return get_sequence(old_pos, pos);
  }
private:
  char pos;

  inline static const std::map<char, std::pair<int , int>> char_to_2dpos {
    {'0', {1, 3}},
    {'1', {0, 2}},
    {'2', {1, 2}},
    {'3', {2, 2}},
    {'4', {0, 1}},
    {'5', {1, 1}},
    {'6', {2, 1}},
    {'7', {0, 0}},
    {'8', {1, 0}},
    {'9', {2, 0}},
    {'A', {2, 3}}
  };
};

void test_numerical_get_sequence(){
  std::vector<char> allowed_chars {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A'};

  for(char& c1 : allowed_chars){
    for(char& c2 : allowed_chars){
        std::cout << c1  << " -> " << c2 << ": " << NumericalKeypad::get_sequence(c1, c2) << "\n";
    }
  }
}

// ===== Directional Keypad =====
/**
 *  ^A
 * <v>
 */
class DirectionalKeypad {
public: 

  DirectionalKeypad(char start) : pos(start) { }
  static std::string get_sequence(char from, char to){
    const static std::map<std::pair<int, int>, std::string> xy_to_sequence {
      {{-1,-1}, "<^"},
      {{0,-1}, "^"},
      {{1,-1}, ">^"},
      {{2,-1}, ">>^"},
      {{-2,0}, "<<"},
      {{-1,0}, "<"},
      {{0,0}, ""},
      {{1,0}, ">"},
      {{2,0}, ">>"},
      {{-2, 1}, "<v<"},
      {{-1,1}, "v<"},
      {{0,1}, "v"},
      {{1,1}, "v>"},
    };
    
    auto from2dpos = char_to_2dpos.at(from);
    auto to2dpos = char_to_2dpos.at(to);

    int move_x = to2dpos.first - from2dpos.first;
    int move_y = to2dpos.second - from2dpos.second;

    return xy_to_sequence.at({move_x, move_y});
  }

  std::string move_to(char to){
    char old_pos = pos;
    pos = to;
    return get_sequence(old_pos, pos);
  }
private:
  char pos;

  inline static const std::map<char, std::pair<int, int>> char_to_2dpos {
    {'^', {1, 0}},
    {'A', {2, 0}},
    {'<', {0, 1}},
    {'v', {1, 1}},
    {'>', {2, 1}}
  };
};

void test_directional_get_sequence() {
  std::vector<char> allowed_chars  {'^', '<', 'v', '>', 'A'};
  for(char& c1 : allowed_chars){
    for(char& c2 : allowed_chars){
        std::cout << c1  << " -> " << c2 << ": " << DirectionalKeypad::get_sequence(c1, c2) << "\n";
    }
  }
}

std::string get_sequence(std::string sequence, int inbetween_keypads){
  NumericalKeypad numPad {'A'};

  std::string newSequence = "";
  for(const char& c : sequence){
    newSequence += numPad.move_to(c) + "A";
  }

  for(int i = 0; i < inbetween_keypads; ++i){
    DirectionalKeypad dirPad {'A'};
    
    sequence = newSequence;
    newSequence = "";

    for(const char& c : sequence) {
      newSequence += dirPad.move_to(c) + "A";
    }
  }

  return newSequence;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }
  
  std::vector<std::string> sequences = read_input(argv[1]);

  int total_complexity = 0;
  for(const auto& sequence :sequences) {
    std::cout << "    NumKeypad: " << sequence << "\n";
    std::string first_sequence = get_sequence(sequence, 0);
    std::cout << "1st DirKeypad: [Size: " << first_sequence.size() << "]: " << first_sequence <<  "\n";
    std::string second_sequence = get_sequence(sequence, 1);
    std::cout << "2nd DirKeypad: [Size: " << second_sequence.size() << "]: " << second_sequence  << "\n";
    std::string third_sequence = get_sequence(sequence, 2);
    std::cout << "3rd DirKeypad: [Size: " << third_sequence.size() << "]: " << third_sequence  << "\n";
    int num = std::stoi(sequence);
    total_complexity += num * third_sequence.size();
    std::cout << "Length = " << third_sequence.size() << "| Num = " << num << "| Complexity = " << num * third_sequence.size() << "\n";
    std::cout << "\n";
  }
  std::cout << "Total complexity: " << total_complexity << "\n";
  
}