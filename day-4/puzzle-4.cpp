#include <fstream>
#include <iostream>
#include <vector>

int calculateAndMarkMovableRolls(std::vector<std::string>& input);
void removeMovableRolls(std::vector<std::string>& input);

int main() {
    //std::ifstream file("test-input-6-4");
    std::ifstream file("input-4");

    std::string line;
    std::vector<std::string> input;
    while (getline(file, line)) {
        input.push_back(line);
    }

    // PART 1

    const int res1 = calculateAndMarkMovableRolls(input);

    std::cout << "Forklift can access " << res1 << " rolls" << std::endl;

    // PART 2

    int res2 {};
    while (true) {
        const int movable {calculateAndMarkMovableRolls(input)};
        if (movable == 0) {
            break;
        }

        res2 += movable;
        removeMovableRolls(input);
    }
    std::cout << "We can remove maximum " << res2 << " rolls" << std::endl;
}

int calculateAndMarkMovableRolls(std::vector<std::string>& input) {
    int movableRolls = 0;

    for (int r = 0; r < input.size(); r++) {
        for (int c = 0; c < input[0].size(); c++) {
            if (input[r][c] == '@' || input[r][c] == '#') {
                int adjacentRolls = 0;
                if (r > 0) {
                    if (input[r-1][c] == '@' || input[r-1][c] == '#') {
                        adjacentRolls++;
                    }
                    if (c > 0 && (input[r-1][c-1] == '@' || input[r-1][c-1] == '#')) {
                        adjacentRolls++;
                    }
                    if (c < input[0].size() - 1 && (input[r-1][c+1] == '@' || input[r-1][c+1] == '#')) {
                        adjacentRolls++;
                    }
                }
                if (r < input.size() - 1) {
                    if (input[r+1][c] == '@' || input[r+1][c] == '#') {
                        adjacentRolls++;
                    }
                    if (c < input[0].size() - 1 &&  (input[r+1][c+1] == '@' || input[r+1][c+1] == '#')) {
                        adjacentRolls++;
                    }
                    if (c > 0 && (input[r+1][c-1] == '@' || input[r+1][c-1] == '#')) {
                        adjacentRolls++;
                    }
                }
                if (c > 0 && (input[r][c-1] == '@' || input[r][c-1] == '#')) {
                    adjacentRolls++;
                }
                if (c < input[0].size() - 1 && (input[r][c+1] == '@' || input[r][c+1] == '#')) {
                    adjacentRolls++;
                }

                if (adjacentRolls < 4) {
                    movableRolls++;
                    input[r][c] = '#';
                }
            }
        }
    }

    return movableRolls;
}

void removeMovableRolls(std::vector<std::string>& input) {
    for (int r = 0; r < input.size(); r++) {
        for (int c = 0; c < input[0].size(); c++) {
            if (input[r][c] == '#') {
                input[r][c] = '.';
            }
        }
    }
}

