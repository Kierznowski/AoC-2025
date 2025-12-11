#include <fstream>
#include <vector>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>

struct Machine {
    std::vector<bool> currentState;
    std::vector<bool> finalState;
    std::vector<std::string> buttons;
    std::vector<int> finalJoltage;
    std::vector<int> currentJoltage;
};

void partOne(const std::vector<Machine>& machines);
void partTwo(const std::vector<Machine>& machines);
int countBestCombinationForLights(Machine& machine);
void checkState(Machine& machine, std::set<std::string>& clickedButtons, int& minClicks, int index);
void flipSwitches(Machine& machine, const std::string& combination);
int countBestCombinationForJoltage(Machine& machine);
void checkJoltage(Machine& machine, int& clickedButtons, int& minClicks, std::vector<int>& clickCombination, std::set<std::vector<int>>& memo);
void increaseJoltage(Machine& machine, std::string& combination);
void decreaseJoltage(Machine& machine, std::string& combination);
int calculateLowerBound(Machine& machine);

int main() {
    //std::fstream file("test-input-10");
    std::fstream file("input-10");

    std::vector<Machine> machines;

    std::string line;
    while (getline(file, line)) {
        int idx = 0;
        std::stringstream ss(line);
        std::string part;

        Machine machine;
        while (getline(ss, part, ' ')) {
            if (idx++ == 0) {
                for (int i = 0; i < part.length() - 1; i++) {
                    if (part[i] == '.') {
                        machine.finalState.push_back(false);
                        machine.currentState.push_back(false);
                    } else if (part[i] == '#') {
                        machine.finalState.push_back(true);
                        machine.currentState.push_back(false);
                    }
                }
            } else if (idx++ > 0) {
                machine.buttons.push_back(part);
            }
        }
        std::stringstream joltageStream(machine.buttons.back().substr(1, machine.buttons.back().length() - 2));
        machine.buttons.pop_back();
        std::string joltage;
        while (getline(joltageStream, joltage, ',')) {
            machine.finalJoltage.push_back(stoi(joltage));
            machine.currentJoltage.push_back(0);
        }
        machines.push_back(machine);
    }
    partOne(machines);
    partTwo(machines);

    return 0;
}

void partOne(const std::vector<Machine>& machines) {
    int buttonsToClick {};
    for (Machine machine : machines) {
         buttonsToClick += countBestCombinationForLights(machine);
    }
    std::cout << "Sum of buttons to click: " << buttonsToClick << std::endl;
}

void partTwo(const std::vector<Machine>& machines) {
    int buttonsToClick {};

    for (Machine machine : machines) {
        buttonsToClick += countBestCombinationForJoltage(machine);
    }
    std::cout << "Sum of buttons to click to set joltage: " << buttonsToClick << std::endl;
}

int countBestCombinationForLights(Machine& machine) {
    std::set<std::string> clickedButtons;
    int minClicks {std::numeric_limits<int>::max()};
    checkState(machine, clickedButtons, minClicks, 0);

    return minClicks;
}

void checkState(Machine& machine, std::set<std::string>& clickedButtons, int& minClicks, int index) {
    if (machine.finalState == machine.currentState) {
        minClicks = std::min(minClicks, static_cast<int>(clickedButtons.size()));
        return;
    }

    for (int i = index; i < machine.buttons.size(); i++) {
        std::string combination = machine.buttons[i];
        flipSwitches(machine, combination);
        clickedButtons.insert(combination);
        checkState(machine, clickedButtons, minClicks, i + 1);
        flipSwitches(machine, combination);
        clickedButtons.erase(combination);
    }
}

void flipSwitches(Machine& machine, const std::string& combination) {
    std::stringstream ss(combination.substr(1, combination.length() - 2));
    std::string buttonNumber;
    while (getline(ss, buttonNumber, ',')) {
        int num = stoi(buttonNumber);
        if (machine.currentState[num] == false) {
            machine.currentState[num] = true;
        } else {
            machine.currentState[num] = false;
        }
    }
}

int countBestCombinationForJoltage(Machine& machine) {
    std::cout << "START" << std::endl;
    int minClicks {std::numeric_limits<int>::max()};
    int clickedButtons = 0;
    std::set<std::vector<int>> memo;
    std::vector<int> clickCombination;
    checkJoltage(machine, clickedButtons, minClicks, clickCombination, memo);

    return minClicks;
}

void checkJoltage(Machine& machine, int& clickedButtons, int& minClicks, std::vector<int>& clickCombination, std::set<std::vector<int>>& memo) {
    if (memo.find(clickCombination) != memo.end()) {
        return;
    }
    if (clickedButtons < 0) {
        throw std::invalid_argument("clickedButtons < 0");
    }

    for (int i = 0; i < machine.finalJoltage.size(); i++) {
        if (machine.currentJoltage[i] > machine.finalJoltage[i]) {
            memo.insert(clickCombination);
            return;
        }
    }

    int lowerBound = calculateLowerBound(machine);
    if (clickedButtons + lowerBound >= minClicks) {
        memo.insert(clickCombination);
        return;
    }


    if (machine.finalJoltage == machine.currentJoltage) {
        std::cout << "FOUND";
        minClicks = std::min(minClicks, static_cast<int>(clickedButtons));
        return;
    }

    for (int i = 0; i < machine.buttons.size(); i++) {
        std::string combination = machine.buttons[i];
        std::vector<int> tmpJoltage = machine.currentJoltage;
        increaseJoltage(machine, combination);
        clickedButtons++;
        clickCombination.push_back(i);
        checkJoltage(machine, clickedButtons, minClicks, clickCombination, memo);
        clickCombination.pop_back();
        machine.currentJoltage = tmpJoltage;
        clickedButtons--;
    }
}

void increaseJoltage(Machine& machine, std::string& combination) {
    std::stringstream ss(combination.substr(1, combination.length() - 2));
    std::string buttonNumber;
    while (getline(ss, buttonNumber, ',')) {
        int num = stoi(buttonNumber);
        machine.currentJoltage[num]++;
    }
}

void decreaseJoltage(Machine& machine, std::string& combination) {
    std::stringstream ss(combination.substr(1, combination.length() - 2));
    std::string buttonNumber;
    while (getline(ss, buttonNumber, ',')) {
        int num = stoi(buttonNumber);
        machine.currentJoltage[num]--;
    }
}

int calculateLowerBound(Machine& machine) {
    int maxNeeded = 0;

    for (int i = 0; i < machine.finalJoltage.size(); i++) {
        int needed = machine.finalJoltage[i] - machine.currentJoltage[i];
        maxNeeded = std::max(maxNeeded, needed);
    }

    return maxNeeded;
}