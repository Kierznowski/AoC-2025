#include <fstream>
#include <iostream>
#include <set>
#include <vector>

bool checkPattern(long long id);
bool checkForNewPattern(long long id);

int main() {
    //std::fstream file("test-input-6-2");
    std::fstream file("input-2");

    if (!file) {
        std::cerr << "Can't open file" << std::endl;
        return 1;
    }

    std::string tempLine;
    std::vector<std::string> ranges;
    while (std::getline(file, tempLine, ',')) {
        ranges.push_back(tempLine);
    }

    long long res {0};
    for (const std::string& range : ranges) {
        int delimIndex = static_cast<int>(range.find('-'));
        long long start = stoll(range.substr(0, delimIndex));
        long long end = stoll(range.substr(delimIndex + 1));
        for (long long i = start; i <= end; i++) {
            if (checkPattern(i)) {
                res += i;
            }
        }
    }

    std::cout << "Sum of all invalid IDs: " << res << std::endl;

    long long res2 {0};
    std::set<long long> countedIds;
    for (const std::string& range : ranges) {
        int delimIndex = static_cast<int>(range.find('-'));
        long long start = stoll(range.substr(0, delimIndex));
        long long end = stoll(range.substr(delimIndex + 1));
        for (long long i = start; i <= end; i++) {
            if (countedIds.find(i) != countedIds.end()) {
                continue;
            }
            countedIds.insert(i);
            if (checkForNewPattern(i)) {
                res2 += i;
            }
        }
    }

    std::cout << "Sum of all invalid IDs - part 2: " << res2 << std::endl;

    return 0;
}

bool checkPattern(const long long id) {
    const std::string strId = std::to_string(id);
    if (strId.length() % 2 != 0) {
        return false;
    }
    const int length {static_cast<int>(strId.length())};
    for (int i = 0, j = length / 2; j < length; i++, j++) {
        if (strId[i] != strId[j]) {
            return false;
        }
    }
    return true;
}

bool checkForNewPattern(const long long id) {

    const std::string strId = std::to_string(id);
    const int length {static_cast<int>(strId.length())};

    for (int patternLen = 1; patternLen <= length / 2; patternLen++) {
        if (length % patternLen != 0) {
            continue;
        }

        std::string pattern = strId.substr(0, patternLen);
        bool found {true};

        for (int pos = patternLen; pos < length; pos += patternLen) {
            if (strId.substr(pos, patternLen) != pattern) {
                found = false;
                break;
            }
        }

        if (found) {
            return true;
        }
    }
    return false;
}
