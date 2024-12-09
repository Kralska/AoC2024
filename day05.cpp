#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v);

template <class T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &v);

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> pair) {
  os << "(" << pair.first << ", " << pair.second << ")";
  return os;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[";
  for (std::size_t idx = 0; idx < (v.size() != 0 ? v.size() - 1 : 0); ++idx) {
    os << v[idx] << ", ";
  }
  if (v.size() > 0) {
    os << v.back();
  }
  os << "]";
  return os;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &s) {
  os << "[";
  for (auto iter = s.cbegin(); iter != s.cend(); ++iter) {
    os << *iter << ", ";
  }
  os << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const std::map<int, std::vector<int>> &map) {
  os << "[";
  for (auto it = map.cbegin(); it != map.cend(); ++it) {
    os << "{";
    os << it->first;
    os << ": ";
    os << it->second;
    os << "}, ";
  }
  os << "]";
  return os;
}

struct PrintInstructions {
  std::vector<std::pair<int, int>> page_ordering_rules{};
  std::vector<std::vector<int>> updates{};
};

PrintInstructions read_input(std::string file_location) {
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  PrintInstructions print_instructions;
  std::string line;
  while (std::getline(input_file, line) && line != "") {
    std::size_t offset = 0;
    int first = std::stoi(line, &offset);
    int second = std::stoi(line.substr(offset + 1));
    print_instructions.page_ordering_rules.push_back({first, second});
  }
  while (std::getline(input_file, line)) {
    std::size_t idx = 0;
    std::size_t read_chars{};
    std::vector<int> update{};
    while (idx < line.size()) {
      update.push_back(std::stoi(line.substr(idx), &read_chars));
      idx += read_chars + 1;
    }
    print_instructions.updates.push_back(update);
  }
  return print_instructions;
}

// ===== Task 1 =====
std::map<int, std::set<int>>
reduce_page_ordering_rules(const std::vector<std::pair<int, int>> &rules) {
  std::map<int, std::set<int>> page_to_forbidden_pages{};
  for (const auto &rule : rules) {
    page_to_forbidden_pages[rule.second].insert(rule.first);
  }
  return page_to_forbidden_pages;
}

bool is_in_correct_order(const std::vector<int> &page_order,
                         const std::map<int, std::set<int>> &page_to_forbidden_pages) {
  std::set<int> forbidden_pages{};
  
  for (const auto &page : page_order) {
    if (forbidden_pages.contains(page)) {
      return false; 
    }
    try {
      std::set<int> at = page_to_forbidden_pages.at(page);
      std::ranges::for_each(at, [&](const auto& a){forbidden_pages.insert(a);});
    }
    catch(const std::out_of_range& e){} // Page not in map? No problem!
  }
  return true;
}

int get_sum_of_middle_pages(const PrintInstructions& print_instructions) {
  const auto reduced_rules = reduce_page_ordering_rules(print_instructions.page_ordering_rules);
  int sum = 0;
  for(const auto& print_order : print_instructions.updates) {
    if(is_in_correct_order(print_order, reduced_rules)){
      sum += print_order[print_order.size() / 2];
    }
  }
  return sum;
}

// ===== Task 2 =====

void reorder_print_order(std::vector<int>& print_order, const std::map<int, std::set<int>>& reduced_rules) {
  std::vector<std::pair<int, std::set<int>>> update_order;
  std::set<int> forbidden_pages{};
  std::vector<int> pages_to_reinsert{};
  
  for (const auto &page : print_order) {
    if (forbidden_pages.contains(page)) {
      pages_to_reinsert.push_back(page); 
    }
    try {
      std::set<int> at = reduced_rules.at(page);
      std::ranges::for_each(at, [&](const auto& a){forbidden_pages.insert(a);});
    }
    catch(const std::out_of_range& e){} // Page not in map? No problem!
  }
}

int get_sum_of_reordered_middle_pages(const PrintInstructions& print_instructions) {
  const auto reduced_rules = reduce_page_ordering_rules(print_instructions.page_ordering_rules);
  
  int sum = 0;
  for(auto print_order : print_instructions.updates) {
    if(is_in_correct_order(print_order, reduced_rules)){
      continue;
    }
    reorder_print_order(print_order, reduced_rules);
    sum += print_order[print_order.size() / 2];
  }
  return sum;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  PrintInstructions print_instructions = read_input(argv[1]);

  // Task 1 : Correctly ordered pages
  int sum_of_middle_pages = get_sum_of_middle_pages(print_instructions);
  std::cout << "Sum of middle pages: " << sum_of_middle_pages << "\n";
  // Task 2 :

  // Test: C++ Ranges and Views
}