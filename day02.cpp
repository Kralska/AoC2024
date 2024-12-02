#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<int> readReport(std::string line) {
    std::size_t idx = 0, increaseIdxBy = 0;
    std::vector<int> report {};
    while(idx < line.size()){
        int level = std::stoi(line.substr(idx), &increaseIdxBy);
        idx += increaseIdxBy;
        report.push_back(level);
    }
    return report;
}

bool checkReport(const std::vector<int>& report, std::vector<std::function<bool(const std::vector<int>&)>> conditions) {
    for(auto condition : conditions){
        if(!condition(report)) {
            return false;
        }
    }
    return true;
}

// ===== Task 01 =====
bool allIncreasingOrAllDecreasing(const std::vector<int>& report){
    // Nothing to check if 0 or 1 elements
    if (report.size() <= 1) {
        return true;
    }

    bool allIncreasing = true, allDecreasing = true;
    int lastValue = *report.begin();
    std::vector<int>::const_iterator iter = ++report.begin();
    while(iter != report.end()){
        if(lastValue >= *iter){
            allDecreasing = false;
        }
        if(lastValue <= *iter){
            allIncreasing = false;
        }
        lastValue = *iter;
        iter++;
    }
    return allIncreasing || allDecreasing;
}

bool adjacentLevelsDifferByAtLeast(const std::vector<int>& report, int min) {
    // Nothing to check if 0 or 1 elements
    if(report.size() <= 1){
        return true;
    }

    int lastValue = *report.begin();
    std::vector<int>::const_iterator iter = ++report.begin();
    while(iter != report.end()) {
        if(std::abs(lastValue - *iter) < min){
            return false;
        }
        lastValue = *iter;
        iter++;
    }
    return true;
}

bool adjacentLevelsDifferByAtMost(const std::vector<int>& report, int max) {
    // Nothing to check if 0 or 1 elements
    if(report.size() <= 1){
        return true;
    }

    int lastValue = *report.begin();
    std::vector<int>::const_iterator iter = ++report.begin();
    while(iter != report.end()) {
        if(std::abs(lastValue - *iter) > max){
            return false;
        }
        lastValue = *iter;
        iter++;
    }
    return true;
}

bool isReportSafe(const std::vector<int>& report){
    using namespace std::placeholders;
    const static std::vector<std::function<bool(const std::vector<int>&)>> conditions {
        allIncreasingOrAllDecreasing,
        std::bind(adjacentLevelsDifferByAtLeast, _1, 1),
        std::bind(adjacentLevelsDifferByAtMost, _1, 3)    
    };
    return checkReport(report, conditions);
}

int totalSafeReports(const std::vector<std::vector<int>>& reports){
    return std::count_if(
        reports.begin(),
        reports.end(),
        isReportSafe
    ); 
}

// ===== Task 2 =====
bool isReportSafeWithProblemDampener(const std::vector<int>& report) {
    for(int idx = 0; idx < report.size(); ++idx) {
        auto report_copy = report;
        report_copy.erase(report_copy.begin() + idx);
        if(isReportSafe(report_copy)) {
            return true;
        }
    }
    return false;
}

int totalSafeReportsWithDampener(const std::vector<std::vector<int>>& reports) {
    return std::count_if(
        reports.begin(),
        reports.end(),
        isReportSafeWithProblemDampener
    );    
}

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
    std::vector<std::vector<int>> reports{};
    while( std::getline(input_file, line) ) {
        reports.push_back(readReport(line));
    }

    // Task 1
    int safe_reports = totalSafeReports(reports);
    std::cout << "Total Safe Reports: " << safe_reports << "\n";

    // Task 2
    int safe_reports_with_problem_dampener = totalSafeReportsWithDampener(reports);
    std::cout << "Total Safe Reports with problem dampener: " << safe_reports_with_problem_dampener << "\n";
}