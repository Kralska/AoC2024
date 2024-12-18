#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static const int kNUM_REGISTERS = 3;
static const std::string instruction_pointer_past_program =
    "Chronospatial computer read an opcode past the end of the program! "
    "Halting!";
namespace Registers {
static const int A = 0;
static const int B = 1;
static const int C = 2;
} // namespace Registers

int get_val_of_combo_operand(int combo_operand,
                             std::array<unsigned int, kNUM_REGISTERS> &registers) {
  assert(combo_operand >= 0);
  assert(combo_operand < 8);

  if (0 <= combo_operand && combo_operand <= 3) {
    return combo_operand;
  }
  if (combo_operand == 7) {
    throw std::invalid_argument(
        "Combo operand 7 should not appear in valid programs.");
  }
  return registers.at(combo_operand - 4);
}

// Opcode 0, 6, 7
void letter_dv(unsigned int &instruction_pointer,
               std::vector<unsigned int> &program,
               std::array<unsigned int, kNUM_REGISTERS> &registers,
               std::stringstream&,
               const int store_in) {
  if (instruction_pointer + 1 >= program.size()) {
    throw std::length_error(instruction_pointer_past_program);
  }

  int numerator = registers.at(Registers::A);
  double denominator = std::pow(
      2.0,
      get_val_of_combo_operand(program.at(instruction_pointer + 1), registers));
  int result = std::trunc(numerator / denominator);
  registers.at(store_in) = result;

  instruction_pointer += 2;
}

// Opcode 1
void bxl(unsigned int &instruction_pointer, std::vector<unsigned int> &program,
         std::array<unsigned int, kNUM_REGISTERS> &registers, std::stringstream&) {
  if (instruction_pointer + 1 >= program.size()) {
    throw std::length_error(instruction_pointer_past_program);
  }

  registers.at(Registers::B) =
      registers.at(Registers::B) ^ program.at(instruction_pointer + 1);

  instruction_pointer += 2;
}

// Opcode 2
void bst(unsigned int &instruction_pointer, std::vector<unsigned int> &program,
         std::array<unsigned int, kNUM_REGISTERS> &registers, std::stringstream&) {
  if (instruction_pointer + 1 >= program.size()) {
    throw std::length_error(instruction_pointer_past_program);
  }

  registers.at(Registers::B) =
      get_val_of_combo_operand(program.at(instruction_pointer + 1), registers) %
      8;

  instruction_pointer += 2;
}

// Opcode 3
void jnz(unsigned int &instruction_pointer, std::vector<unsigned int> &program,
         std::array<unsigned int, kNUM_REGISTERS> &registers, std::stringstream&) {
  if (instruction_pointer + 1 >= program.size()) {
    throw std::length_error(instruction_pointer_past_program);
  }

  if (registers.at(Registers::A) == 0) {
    instruction_pointer += 2;
    return;
  }

  instruction_pointer = program.at(instruction_pointer + 1);
}

// Opcode 4
void bxc(unsigned int &instruction_pointer, std::vector<unsigned int> &program,
         std::array<unsigned int, kNUM_REGISTERS> &registers, std::stringstream&) {
  if (instruction_pointer + 1 >= program.size()) {
    throw std::length_error(instruction_pointer_past_program);
  }

  registers.at(Registers::B) =
      registers.at(Registers::B) ^ registers.at(Registers::C);

  instruction_pointer += 2;
}

// Opcode 5
void out(unsigned int &instruction_pointer, std::vector<unsigned int> &program,
         std::array<unsigned int, kNUM_REGISTERS> &registers, std::stringstream& out) {
  if (instruction_pointer + 1 >= program.size()) {
    throw std::length_error(instruction_pointer_past_program);
  }
  out << get_val_of_combo_operand(program.at(instruction_pointer + 1),
                                        registers) %
                   8 << ",";

  instruction_pointer += 2;
}

using namespace std::placeholders;

class ChronospatialComputer {
public:
  ChronospatialComputer() : registers_{0, 0, 0} {}
  ChronospatialComputer(std::vector<unsigned int> program)
      : program_(program), registers_{0, 0, 0} {}
  ChronospatialComputer(std::array<unsigned int, kNUM_REGISTERS> registers)
      : registers_(registers) {}
  ChronospatialComputer(std::vector<unsigned int> program,
                        std::array<unsigned int, kNUM_REGISTERS> registers)
      : program_(program), registers_(registers) {}

  void load_program(std::vector<unsigned int> program) {
    program_ = program;
    instruction_pointer_ = 0;
  }

  void load_registers(std::array<unsigned int, kNUM_REGISTERS> registers) {
    registers_ = registers;
  }

  void run() {
    while (instruction_pointer_ < program_.size()) {
      try {
        auto& func = instructions[program_.at(instruction_pointer_)];
        func(instruction_pointer_, program_, registers_, output_);
      } catch (const std::length_error &err) {
        return;
      }
    }
  }

  void reset() {
    instruction_pointer_ = 0;
    output_.str(std::string());
  }

  std::string get_output() const{
    auto str = output_.str();
    if(str.ends_with(",")){
        return str.substr(0, str.size() - 1);
    }
    return str;
  }

private:
  friend std::ostream &operator<<(std::ostream &out,
                                  const ChronospatialComputer &);

  std::array<unsigned int, kNUM_REGISTERS> registers_;

  unsigned int instruction_pointer_ = 0;
  std::vector<unsigned int> program_{};

  const std::array<std::function<void(unsigned int &, std::vector<unsigned int> &,
                                    std::array<unsigned int, kNUM_REGISTERS> &, std::stringstream&)>, 8>
      instructions {
          std::bind(letter_dv, _1, _2, _3, _4,  Registers::A),
          bxl,
          bst,
          jnz,
          bxc,
          out,
          std::bind(letter_dv, _1, _2, _3, _4, Registers::B),
          std::bind(letter_dv, _1, _2, _3, _4, Registers::C)
      };

  std::stringstream output_ {};
};

std::ostream &operator<<(std::ostream &os,
                         const ChronospatialComputer &computer) {
  os << "{";
  os << "Instruction Pointer: " << computer.instruction_pointer_ << ", ";
  os << "Program: " << "[";
  std::ranges::for_each(computer.program_,
                        [&](const auto &val) { os << val << ", "; });
  os << "]" << ", ";
  os << "Registers: ";
  std::ranges::for_each(computer.registers_,
                        [&](const auto &val) { os << val << ", "; });
  os << "]" << ", ";
  os << "Output: " << "\"" << computer.get_output() << "\"";
  os << "}";

  return os;
}

void test_computer() {
  ChronospatialComputer computer{{2, 6}, {0, 0, 9}};
  computer.run();
  std::cout << computer << "\n";

  computer.reset();
  computer.load_program({5, 0, 5, 1, 5, 4});
  computer.load_registers({10, 0, 0});
  computer.run();
  std::cout << computer << "\n";

  computer.reset();
  computer.load_program({0, 1, 5, 4, 3, 0});
  computer.load_registers({2024, 0, 0});
  computer.run();
  std::cout << computer << "\n";

  computer.reset();
  computer.load_program({1, 7});
  computer.load_registers({0, 29, 0});
  computer.run();
  std::cout << computer << "\n";

  computer.reset();
  computer.load_program({4, 0});
  computer.load_registers({0, 2024, 43690});
  computer.run();
  std::cout << computer << "\n";
}

ChronospatialComputer read_input(std::string file_location){
  std::ifstream input_file;
  input_file.open(file_location);
  if (!input_file.is_open()) {
    throw std::invalid_argument("Unable to open file at " + file_location);
  }

  std::string line;
  std::getline(input_file, line);
  unsigned int register_a = static_cast<unsigned int>(std::stoi(line.substr(line.find("A: ") + 3)));
  std::getline(input_file, line);
  unsigned int register_b = static_cast<unsigned int>(std::stoi(line.substr(line.find("B: ") + 3)));
  std::getline(input_file, line);
  unsigned int register_c = static_cast<unsigned int>(std::stoi(line.substr(line.find("C: ") + 3)));
  std::getline(input_file, line);
  std::getline(input_file, line);
  line = line.substr(line.find("Program: ") + 9);
  std::vector<unsigned int> program {};
  int idx = 0;
  std::size_t read_chars {};
  while(idx < line.size()) {
    unsigned int operand = static_cast<unsigned int>(std::stoi(line.substr(idx), &read_chars));
    idx += read_chars + 1;

    program.push_back(operand);
  }

  return {program, {register_a, register_b, register_c}};
  
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Not enough arguments! " << "\n";
    return 0;
  }

  //test_computer();

  // First argument is input file location
  ChronospatialComputer computer = read_input(argv[1]);
  std::cout << computer << "\n";

  // Task 1
  computer.run();
  std::cout << "Output from computer run: " << computer.get_output() << "\n";

  // Task 2
  for(unsigned int i = 0; i < 100; ++i) {
    computer.reset();
    computer.load_registers({i, 0, 0});
    computer.run();
    std::cout << "Start " << i << ": " << computer.get_output() << "\n";
  }
}