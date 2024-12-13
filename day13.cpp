#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Equation {
  long long x, y, z;

  Equation(long long x, long long y, long long z) : x(x), y(y), z(z) {}
};

struct EquationSet {
  Equation eq1, eq2;

  EquationSet(long long x1, long long y1, long long z1, long long x2,
              long long y2, long long z2)
      : eq1(x1, y1, z1), eq2(x2, y2, z2) {}
};

std::vector<EquationSet> read_input(std::string file_location) {
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::string line;
  long long x1, y1, z1, x2, y2, z2;
  std::vector<EquationSet> equations{};
  while (std::getline(input_file, line)) {
    if (line.starts_with("Button A:")) {
      x1 = std::stoll(line.substr(line.find("X+") + 2));
      x2 = std::stoll(line.substr(line.find("Y+") + 2));
    }
    if (line.starts_with("Button B:")) {
      y1 = std::stoll(line.substr(line.find("X+") + 2));
      y2 = std::stoll(line.substr(line.find("Y+") + 2));
    }
    if (line.starts_with("Prize:")) {
      z1 = std::stoll(line.substr(line.find("X=") + 2));
      z2 = std::stoll(line.substr(line.find("Y=") + 2));
      equations.push_back(EquationSet(x1, y1, z1, x2, y2, z2));
    }
  }
  return equations;
}

long long get_required_tokens(const EquationSet &equation_set) {
  const auto &eq1 = equation_set.eq1;
  const auto &eq2 = equation_set.eq2;
  long long numerator = (eq1.x * eq2.z - eq2.y * eq1.z);
  long long denominator = (eq1.y * eq2.x - eq1.x * eq2.y);
  if (denominator == 0) {
    std::cerr << "Equation is linearly dependent!" << "\n";
    throw std::logic_error("Equations linearly dependant!");
  }
  double a_double = ((double)(eq1.y * eq2.z - eq2.y * eq1.z)) /
                    (eq1.y * eq2.x - eq1.x * eq2.y);
  long long a = std::round(a_double);
  if (a_double - a != 0) {
    return 0;
  }

  double b_double = ((double)eq1.z - eq1.x * a) / eq1.y;
  long long b = std::round(b_double);
  if (b_double - b != 0) {
    return 0;
  }
  return a * 3 + b;
}

// ===== Task 1 =====
long long get_total_tokens(const std::vector<EquationSet> &equations) {
  long long sum = 0;
  for (const auto &eq : equations) {
    sum += get_required_tokens(eq);
  }
  return sum;
}

// ===== Task 2 =====
long long get_total_tokens_with_error(std::vector<EquationSet> equations) {
  std::ranges::for_each(equations, [](auto &set) {
    set.eq1.z += 10000000000000;
    set.eq2.z += 10000000000000;
  });
  long long sum = 0;
  for (const auto &eq : equations) {
    sum += get_required_tokens(eq);
  }
  return sum;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  // First argument is input file location
  const std::vector<EquationSet> equations = read_input(argv[1]);

  // Task 1
  long long total_presses = get_total_tokens(equations);
  std::cout << "Total tokens to get the most prices: " << total_presses << "\n";

  // Task 2
  long long total_presses_with_conversion_error =
      get_total_tokens_with_error(equations);
  std::cout << "Total tokens to get the most prices with conversion error: "
            << total_presses_with_conversion_error << "\n";
}