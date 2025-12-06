#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

int main() {
    //std::fstream file("test-input-6");
    std::fstream file("input-6");

    std::string line;
    std::vector<std::string> homework;

    while (getline(file, line)) {
        homework.push_back(line);
    }
    file.close();

    // PART 1
    std::vector<long long> results;
    std::string operatorsRow = homework[homework.size() - 1];
    std::vector<char> operators;
    for (int i = 0; i < operatorsRow.size(); i++) {
        if (operatorsRow[i] != ' ') {
            operators.push_back(operatorsRow[i]);
        }
    }

    for (int h = 0; h < homework.size() - 1; h++) {
        std::string row = homework[h];
        int pos = 0;
        std::string buffer;

        for (int i = 0; i < row.size(); i++) {
            if (row[i] == ' ' && !buffer.empty()) {
                int num = stoi(buffer);
                buffer.clear();
                if (h == 0) {
                    results.push_back(num);
                } else if (operators[pos] == '+') {
                    results[pos] += num;
                } else {
                    results[pos] *= num;
                }
                pos++;
            } else if (row[i] != ' '){
                buffer += row[i];
            }
        }
        if (!buffer.empty()) {
            if (h == 0) {
                results.push_back(stoi(buffer));
            } else if (operators[pos] == '+') {
                results[pos] += stoi(buffer);
            } else {
                results[pos] *= stoi(buffer);
            }
        }
    }

    long long grantTotal = 0;
    for (long long res : results) {
        std::cout << res << std::endl;
        grantTotal += res;
    }

    std::cout << "Grand total: " << grantTotal << std::endl;

    // PART 2
    long long grandTotal2 = 0;

    int longestRow = 0;
    for (std::string row : homework) {
        if (row.size() > longestRow) {
            longestRow = row.size();
        }
    }

    std::vector<int> parsedNums;
    for (int col = longestRow - 1; col >= 0; col--) {
        std::string num;
        for (int row = 0; row < homework.size() - 1; row++) {
            if (col >= homework[row].size()) {
                continue;
            }
            if (homework[row][col] != ' ') {
                num += homework[row][col];
            }
        }
        parsedNums.push_back(std::stoi(num));
        if (homework[homework.size() - 1][col] == '+') {
            long long tempRes = 0;
            for (int number : parsedNums) {
                tempRes += number;
            }
            grandTotal2 += tempRes;
            parsedNums.clear();
            col--;
            num.clear();
        } else if (homework[homework.size() - 1][col] == '*') {
            long long tempRes = 1;
            for (long long number : parsedNums) {
                tempRes *= number;
            }
            grandTotal2 += tempRes;
            parsedNums.clear();
            col--;
            num.clear();
        }
    }

    std::cout << "Real Grand total: " << grandTotal2 << std::endl;

    return 0;
}