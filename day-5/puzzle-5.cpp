#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

bool foundInDB(std::vector<std::string>& db, long long);
void normalizeDB(std::vector<std::string>& db, std::vector<std::pair<long long, long long>>& normalizedDB);

int main() {
    //std::fstream file("test-input-6-5");
    std::fstream file("input-5");

    std::string line;
    std::vector<std::string> database;
    std::vector<long long> ingredientId;

    bool ingredientList = false;

    while (getline(file, line)) {
        if (line.empty()) {
            ingredientList = true;
        } else if (ingredientList) {
            ingredientId.push_back(std::stoll(line));
        } else {
            database.push_back(line);
        }
    }

    // PART 1

    int freshIngredientsCount {0};
    for (long long id : ingredientId) {
        if (foundInDB(database, id)) {
            freshIngredientsCount++;
        }

    }

    std::cout << "There are " << freshIngredientsCount << " fresh ingredients from the list." << std::endl;

    // PART 2

    sort(database.begin(), database.end(), [](const std::string& first, const std::string& second) {
        const long long firstRow = std::stoll(first.substr(0, first.find('-')));
        const long long secondRow = std::stoll(second.substr(0, second.find('-')));
        return firstRow < secondRow;
    });

    std::vector<std::pair<long long, long long>> normalizedDB;
    normalizeDB(database, normalizedDB);
    long long dbRows {0};
    for (std::pair pair : normalizedDB) {
        dbRows += pair.second - pair.first + 1;
    }

    std::cout << "There are " << dbRows << " fresh ingredients in whole DB." << std::endl;

    return 0;
}

bool foundInDB(std::vector<std::string>& db, const long long id) {
    for (std::string& row : db) {
        const int delim = static_cast<int>(row.find('-'));
        const long long start = stoll(row.substr(0, delim));
        const long long end = stoll(row.substr(delim + 1));
        if (id >= start && id <= end) {
            return true;
        }
    }
    return false;
}

void normalizeDB(std::vector<std::string>& db, std::vector<std::pair<long long, long long>>& normalizedDB) {
    const int delimiter = static_cast<int>(db[0].find('-'));
    long long a = std::stoll(db[0].substr(0, delimiter));
    long long b = std::stoll(db[0].substr(delimiter + 1));
    normalizedDB.emplace_back(a, b);

    for (int i = 1; i < db.size(); i++) {
        const int delim = static_cast<int>(db[i].find('-'));
        long long secStart = std::stoll(db[i].substr(0, delim));
        long long secEnd = std::stoll(db[i].substr(delim + 1));

        long long firstStart = normalizedDB.at(normalizedDB.size() - 1).first;
        long long firstEnd = normalizedDB.at(normalizedDB.size() - 1).second;

        if (secStart <= firstEnd) {
            normalizedDB.pop_back();
            if (secEnd > firstEnd) {
                normalizedDB.emplace_back(firstStart, secEnd);
            } else {
                normalizedDB.emplace_back(firstStart, firstEnd);
            }
        } else {
            normalizedDB.emplace_back(secStart, secEnd);
        }
    }
}


