#include <fstream>
#include <functional>
#include <iostream>
#include <cctype>
#include <regex>

// ===== State Machine Implementation =====
struct State {
    const std::vector<std::pair<std::function<bool(char)>, int>> transitions;

    State(const std::vector<std::pair<std::function<bool(char)>, int>>& transitions) : transitions(transitions) { }
};

class StateMachine {
public:
    StateMachine(const std::vector<State>& states, int start) : states_(states), start_(start) {
        current_ = start;
     }

    // Changes the State Machines state and returns the idx of the current state
    int toNextState(char c) {
        for(const auto& transition : states_[current_].transitions) {
            if(transition.first(c)) {
                current_ = transition.second;
                return current_;
            }
        }
        current_ = start_;
        return start_;
    }
private:
    const std::vector<State> states_;
    const int start_;
    int current_;
};

// ===== State Machines =====
StateMachine getMulInstructionsStateMachine() {
    const static std::vector<State> states {
        State({{[](unsigned char c){return c == 'm';}, 1}}),
        State({
            {[](unsigned char c){return c == 'u';}, 2},
            {[](unsigned char c){return c == 'm';}, 1}
            
        }),
        State({
            {[](unsigned char c){return c == 'l';}, 3},
            {[](unsigned char c){return c == 'm';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == '(';}, 4},
            {[](unsigned char c){return c == 'm';}, 1}    
        }),
        State({
            {[](unsigned char c){return std::isdigit(c) != 0;}, 5},
            {[](unsigned char c){return c == 'm';}, 1}
        }),
        State({
            {[](unsigned char c){return std::isdigit(c) != 0;}, 5},
            {[](unsigned char c){return c == ',';}, 6},
            {[](unsigned char c){return c == 'm';}, 1}
        }),
        State({
            {[](unsigned char c){return std::isdigit(c) != 0;}, 7},
            {[](unsigned char c){return c == 'm';}, 1}
        }),
        State({
            {[](unsigned char c){return std::isdigit(c) != 0;}, 7},
            {[](unsigned char c){return c == ')';}, 8}, 
            {[](unsigned char c){return c == 'm';}, 1}
        }),
        State({{[](unsigned char c){return c == 'm';}, 1}})      
    };
    return StateMachine(states, 0);
}

StateMachine getDoStateMachine(){
    const static std::vector<State> states {
        State({{[](unsigned char c){return c == 'd';}, 1}}),
        State({
            {[](unsigned char c){return c == 'o';}, 2},
            {[](unsigned char c){return c == 'd';}, 1}
            
        }),
        State({
            {[](unsigned char c){return c == '(';}, 3},
            {[](unsigned char c){return c == 'd';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == ')';}, 4},
            {[](unsigned char c){return c == 'd';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == 'd';}, 1}    
        })
    };
    return StateMachine(states, 0);
}

StateMachine getDontStateMachine(){
    const static std::vector<State> states {
        State({{[](unsigned char c){return c == 'd';}, 1}}),
        State({
            {[](unsigned char c){return c == 'o';}, 2},
            {[](unsigned char c){return c == 'd';}, 1}
            
        }),
        State({
            {[](unsigned char c){return c == 'n';}, 3},
            {[](unsigned char c){return c == 'd';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == '\'';}, 4},
            {[](unsigned char c){return c == 'd';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == 't';}, 5},
            {[](unsigned char c){return c == 'd';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == '(';}, 6},
            {[](unsigned char c){return c == 'd';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == ')';}, 7},
            {[](unsigned char c){return c == 'd';}, 1}    
        }),
        State({
            {[](unsigned char c){return c == 'd';}, 1}    
        })
    };
    return StateMachine(states, 0);
}

// ===== Task 1 =====
int calculateSumOfAllMultiplications(const std::string& memory) {
    StateMachine sm = getMulInstructionsStateMachine();

    int sum = 0;
    std::string strLeftNum = "", strRightNum = "";
    int leftNum = 0, rightNum = 0;
    for(const auto& c : memory) {
        int currentState = sm.toNextState(c);
        if (currentState == 1) {
            strLeftNum = "";
            strRightNum = "";
        }
        else if (currentState == 5) {
            strLeftNum += c;
        } else if (currentState == 6) {
            leftNum = std::stoi(strLeftNum);
        } else if (currentState == 7) {
            strRightNum += c;
        } else if (currentState == 8) {
            rightNum = std::stoi(strRightNum);
            sum += leftNum * rightNum;
        }
    }
    return sum;
}

int sumOfInstruction(const std::string& instruction) {
    std::size_t readChars {};
    int left = std::stoi(instruction.substr(4), &readChars);
    int right = std::stoi(instruction.substr(5 + readChars));
    return left * right;
}

int calculateSumOfAllMultiplicationsRegex(const std::string& memory){
    std::regex instructions_regex("mul\\(\\d{1,3},\\d{1,3}\\)");
    auto instructions_begin = std::sregex_iterator(memory.begin(), memory.end(), instructions_regex);
    auto instructions_end = std::sregex_iterator();

    int sum = 0;
    for(std::sregex_iterator i = instructions_begin; i != instructions_end; ++i) {
        std::string match_str = (*i).str();
        sum += sumOfInstruction(match_str);
    }
    return sum;
}

// ===== Task 2 =====
int calculateSumOfAllEnabledInstructions(const std::string& memory){
    StateMachine smMul = getMulInstructionsStateMachine();
    StateMachine smDo = getDoStateMachine();
    StateMachine smDont = getDontStateMachine();

    bool enabled = true;
    int sum = 0;
    std::string strLeftNum = "", strRightNum = "";
    int leftNum = 0, rightNum = 0;
    for(const auto& c : memory) {
        // Do
        int stateDo = smDo.toNextState(c);
        if(stateDo == 4){
            enabled = true;
        }
        // Dont
        int stateDont = smDont.toNextState(c);
        if(stateDont == 7){
            enabled = false;
        }
        // Mul
        int stateMul = smMul.toNextState(c);
        if (stateMul == 1) {
            strLeftNum = "";
            strRightNum = "";
        }
        else if (stateMul == 5) {
            strLeftNum += c;
        } else if (stateMul == 6) {
            leftNum = std::stoi(strLeftNum);
        } else if (stateMul == 7) {
            strRightNum += c;
        } else if (stateMul == 8) {
            if(enabled){
                rightNum = std::stoi(strRightNum);
                sum += leftNum * rightNum;
            }
        }
    }
    return sum;
}

int calculateSumOfAllEnabledInstructionsRegex(const std::string& memory){
    std::regex instructions_regex("mul\\(\\d{1,3},\\d{1,3}\\)|do\\(\\)|don't\\(\\)");
    auto instructions_begin = std::sregex_iterator(memory.begin(), memory.end(), instructions_regex);
    auto instructions_end = std::sregex_iterator();

    bool enabled = true;
    int sum = 0;
    for(std::sregex_iterator i = instructions_begin; i != instructions_end; ++i) {
        std::string match_str = (*i).str();
        if(match_str == "do()"){
            enabled = true;
        } else if(match_str == "don't()") {
            enabled = false;
        } else {
            if(enabled) {
                sum += sumOfInstruction(match_str);
            }
        }   
    }
    return sum;
}

int main(int argc, char **argv)
{
    std::ifstream input_file;
    if(argc < 2){
        std::cout << "Not enough arguments! " << "\n";
        return 0;
    }

    // First argument is input file location
    input_file.open(argv[1]);
    if(!input_file.is_open())
    {
        std::cerr << "Unable to open file." << std::endl;
        return 0;
    }

    // Read input
    std::string line;
    std::string memory {};
    while( std::getline(input_file, line) ) {
        memory += line;
    }

    // Task 1
    int sumOfInstructions = calculateSumOfAllMultiplications(memory);
    std::cout << "Sum of Instructions: " << sumOfInstructions << "\n";

    int sumOfInstructionsRegex = calculateSumOfAllMultiplicationsRegex(memory);
    std::cout << "Sum of instructions(Regex): " << sumOfInstructionsRegex << "\n";

    // Task 2
    int sumOfEnabledInstructions = calculateSumOfAllEnabledInstructions(memory);
    std::cout << "Sum of enabled instructions: " << sumOfEnabledInstructions  << "\n";

    int sumOfEnabledInstructionsRegex = calculateSumOfAllEnabledInstructionsRegex(memory);
    std::cout << "Sum of enabled instructions(Regex): " << sumOfEnabledInstructionsRegex << "\n";
}