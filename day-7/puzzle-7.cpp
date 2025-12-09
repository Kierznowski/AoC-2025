#include <fstream>
#include <iostream>
#include <set>
#include <vector>

void partOne(const std::vector<std::string>& datasheet);
void partTwo(const std::vector<std::string>& datasheet);
void observe(int& beams, int row, int col, const std::vector<std::string>& datasheet);

int main() {
    //std::fstream file("test-input-7");
    std::fstream file("input-7");

    std::string line;
    std::vector<std::string> datasheet;

    while (getline(file, line)) {
        datasheet.push_back(line);
    }
    file.close();

    partOne(datasheet);
    partTwo(datasheet);
}

void partOne(const std::vector<std::string>& datasheet) {
    int splits {0};

    const int startPos =  static_cast<int>(datasheet[0].find('S'));
    std::set<int> beams;
    beams.insert(startPos);

    for (int i = 1; i < datasheet.size() - 1; i++) {
        for (int j = 0; j < datasheet[i].size(); j++) {
            if (datasheet[i][j] == '^' && beams.find(j) != beams.end()) {
                splits++;
                beams.erase(j);
                beams.insert(j+1);
                beams.insert(j - 1);
            }
        }
    }

    std::cout << "There was " << splits << " beam splits" << std::endl;
}

void partTwo(const std::vector<std::string>& datasheet) {
    const int startPos =  static_cast<int>(datasheet[0].find('S'));
    long long beams[datasheet[0].size()];
    for (int i = 0; i < datasheet[0].size(); i++) {
        beams[i] = 0;
    }
    beams[startPos]++;
    for (int row = 2; row < datasheet.size(); row+=2) {
        for (int col = 0; col < datasheet[row].size(); col++) {
            if (datasheet[row][col] == '^' && beams[col] > 0) {
                beams[col + 1] += beams[col];
                beams[col - 1] += beams[col];
                beams[col] = 0;
            }
        }
    }
    long long timelines {};
    for (const long long b : beams) {
        timelines += b;
    }
    std::cout << "Particle end up in " << timelines << " timelines." << std::endl;
}