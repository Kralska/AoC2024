#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
    std::ifstream input_file;
    if(argc < 2){
        std::cout << "Not enough arguments! " << "\n";
    }

    // First argument is input file location
    input_file.open(argv[1]);
    if(!input_file.is_open())
    {
        std::cerr << "Unable to open file." << std::endl;
        return(0);
    }

    // Read input
    std::string line;
    std::vector<int> left {}, right {};
    while( std::getline(input_file, line) ) {
        std::size_t idx{};
        int newLeft = std::stoi(line, &idx);
        int newRight = std::stoi(line.substr(idx), &idx);

        left.push_back(newLeft);
        right.push_back(newRight);
    }

    assert(left.size() == right.size());

    // Calculate task 1(Difference)
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    int sum = 0;
    for(int idx = 0; idx < left.size(); idx++) {
        sum += std::abs(left[idx] - right[idx]);
    }
    std::cout << "Total distance between lists: " << sum << "\n";

    // Calculate task 2(Similarity)
    sum = 0;
    for(int number : left) {
        sum += number * std::count(right.begin(), right.end(), number);
    }
    std::cout << "Similarity score: " << sum << "\n";
}