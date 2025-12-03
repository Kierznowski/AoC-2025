#include <fstream>
#include <iostream>
#include <vector>

int calculateJoltage(std::string& bank);
long long calculateJoltage2(std::string& bank);
int findBestBatteryIdx(std::string& bank, int startIdx, int endIdx);

int main() {

    //std::fstream file("test-input-3");
    std::fstream file("input-3");

    std::string line;
    std::vector<std::string> banks;

    while (getline(file, line)) {
        banks.push_back(line);
    }

    // PART 1
    int totalJoltage {};
    for (std::string& bank : banks) {
        totalJoltage += calculateJoltage(bank);
    }

    std::cout << "Total output joltage is: " << totalJoltage << std::endl;

    // PART 2

    long long totalJoltage2 {};
    for (std::string& bank : banks) {
        totalJoltage2 += calculateJoltage2(bank);
    }

    std::cout << "Total output joltage2 is: " << totalJoltage2 << std::endl;

    return 0;
}

int calculateJoltage(std::string& bank) {
    int firsIndex = 0;
    for (int i = 1; i < bank.length() - 1; i++) {
        if (bank[i] - '0' > bank[firsIndex] - '0') {
            firsIndex = i;
        }
    }
    int secondIndex = firsIndex + 1;
    for (int i = firsIndex + 2; i < bank.length(); i++) {
        if (bank[i] - '0' > bank[secondIndex] - '0') {
            secondIndex = i;
        }
    }

    return (bank[firsIndex] - '0') * 10 + bank[secondIndex] - '0';
}

long long calculateJoltage2(std::string& bank) {
    std::string bankJoltage;
    int lastIndex = {0};
    for (int i = 12; i > 0; i--) {
        lastIndex = findBestBatteryIdx(bank, lastIndex, bank.length() - i);
        bankJoltage += bank[lastIndex];
        lastIndex++;
    }
    return stoll(bankJoltage);
}

int findBestBatteryIdx(std::string& bank, int startIdx, int endIdx) {
    int bestIdx = startIdx;
    for (int i = startIdx + 1; i <= endIdx; i++) {
        if (bank[i] - '0' > bank[bestIdx] - '0') {
            bestIdx = i;
        }
    }
    return bestIdx;
}