#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <regex>

// ===== Task 1 =====
int find_words(const std::vector<std::string>& word_search, const std::string& word) {
  
  std::vector<std::string> split_word_search {};
  // Horizontal
  for (const auto &line : word_search) {
    split_word_search.push_back(line);
  }
  // Vertical
  for(int x = 0; x < word_search[0].size(); ++x){
    std::string column {};
    for(int y = 0; y < word_search.size(); ++y){
        column += word_search[y][x];
    }
    split_word_search.push_back(column);
  }
  // Diagonal -> DownRight
  std::vector<std::pair<int, int>> start_idxs {{0,0}};
  for(int x = 1; x < word_search.size(); ++x){
    start_idxs.push_back({0,x});
    start_idxs.push_back({x,0});
  }
  for(const auto& start_idx : start_idxs){
    std::string diag {};
    int offset = 0;
    while(start_idx.first + offset < word_search.size() && start_idx.second + offset < word_search.size()){
        diag += word_search[start_idx.first+offset][start_idx.second+offset];
        ++offset;
    }
    split_word_search.push_back(diag);
  }
  // Diagonal DownLeft
  start_idxs.clear();
  start_idxs.push_back({0, 0});
  for(int x = 1; x < word_search.size(); ++x) {
    start_idxs.push_back({0, x});
    start_idxs.push_back({x, word_search.size()-1});
  }
  for(const auto& start_idx : start_idxs){
    std::string diag {};
    int offset = 0;
    while(start_idx.first + offset < word_search.size() && start_idx.second - offset >= 0){
        diag += word_search[start_idx.first+offset][start_idx.second-offset];
        ++offset;
    }
    split_word_search.push_back(diag);
  }

  std::string regex_string_backward = "";
  for(auto c = word.rbegin(); c != word.rend(); ++c){
    regex_string_backward += *c;
  }
  std::regex word_regex(word);
  std::regex word_regex_backward(regex_string_backward);
  int matches = 0;
  for(const auto& searchable_string : split_word_search) {
    auto words_begin = std::sregex_iterator(searchable_string.begin(), searchable_string.end(), word_regex);
    auto words_end = std::sregex_iterator();
    matches += std::distance(words_begin, words_end);
    auto words_backwards_begin = std::sregex_iterator(searchable_string.begin(), searchable_string.end(), word_regex_backward);
    auto words_backwards_end = std::sregex_iterator();
    matches += std::distance(words_backwards_begin, words_backwards_end);
  }
  return matches;
}

// ===== Task 2 =====
int find_x_mas(std::vector<std::string> word_search) {
    int matches = 0;
    for(int x = 0; x < word_search[0].size() - 2; ++x){
        for(int y = 0; y < word_search.size() - 2; ++y) {
            if(word_search[y+1][x+1] != 'A'){
                continue;
            }
            char upper_left = word_search[y][x];
            char upper_right = word_search[y][x+2];
            char lower_left = word_search[y+2][x];
            char lower_right = word_search[y+2][x+2];

            // Check char correct letters
            if(upper_left != 'M' && upper_left != 'S'){
                continue;
            }
            if(upper_right != 'M' && upper_right != 'S'){
                continue;
            }
            if(lower_left != 'M' && lower_left != 'S'){
                continue;
            }
            if(lower_right != 'M' && lower_right != 'S'){
                continue;
            }
            // Check opposite chars different
            if(upper_left == lower_right){
                continue;
            }
            if(upper_right == lower_left){
                continue;
            }
            matches += 1;
        }
    }
    return matches;
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
  std::string line;
  std::vector<std::string> word_search{};
  while (std::getline(input_file, line)) {
    word_search.push_back(line);
  }

  // Task 1 : XMAS
  std::string word = "XMAS";
  int words_found = find_words(word_search, "XMAS");
  std::cout << "Found word \"" << word << "\" " << words_found << " times." << "\n";

  // Task 2 : X-MAS(Crosses)
  int crosses_found = find_x_mas(word_search);
  std::cout << "Found X-MAS(2 Crossed MAS') " << crosses_found << " times." << "\n";
}