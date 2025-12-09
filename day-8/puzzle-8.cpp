#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <utility>
#include <set>

struct JunctionBox {
    int x = 0;
    int y = 0;
    int z = 0;
    bool connected = false;

    bool operator<(const JunctionBox& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
};

struct Pair {
    JunctionBox box1;
    JunctionBox box2;
    double distance;
};

struct Circuit {
    std::set<JunctionBox> junctionBoxes;
    double circuitDistance = 0;
};

void partOne(std::vector<JunctionBox> map);
void partTwo();
double calculateDistance(JunctionBox junctionBox1, JunctionBox junctionBox2);


int main() {
    //std::fstream file("test-input-8");
    std::fstream file("input-8");

    std::string line;
    std::vector<JunctionBox> map;

    while (getline(file, line)) {
        std::string position;
        int pos[3];
        int i = 0;
        std::stringstream stream(line);
        while (getline(stream, position, ',')) {
            pos[i++] = std::stoi(position);
        }
        JunctionBox junctionBox = {pos[0], pos[1], pos[2]};
        map.push_back(junctionBox);
    }
    file.close();

    partOne(map);
    partTwo();
}

void partOne(std::vector<JunctionBox> map) {
    std::vector<Pair> pairs;
    for (int first = 0; first < map.size(); first++) {
        for (int second = first + 1; second < map.size(); second++) {
            double distance = calculateDistance(map[first], map[second]);
            Pair pair = {map[first], map[second], distance};
            pairs.push_back(pair);
        }
    }
    std::sort(pairs.begin(), pairs.end(), [](Pair p1, Pair p2) {
        return p1.distance < p2.distance;
    });

    std::vector<Circuit> circuits;
    int i = 0;
    Pair lastMergePair;
    bool hasMerge = false;
    while ((circuits.empty() || circuits.size() > 0) && i < pairs.size()) {
        Pair pair = pairs[i++];
        std::sort(circuits.begin(), circuits.end(), [](Circuit c1, Circuit c2) {
            return c1.circuitDistance > c2.circuitDistance;
        });

        int circuit1Index = -1;
        int circuit2Index = -1;

        for (int j = 0; j < circuits.size(); j++) {
            if (circuits[j].junctionBoxes.find(pair.box1) != circuits[j].junctionBoxes.end()) {
                circuit1Index = j;
            }
            if (circuits[j].junctionBoxes.find(pair.box2) != circuits[j].junctionBoxes.end()) {
                circuit2Index = j;
            }
        }

        if (circuit1Index == -1 && circuit2Index == -1) {
            Circuit circuit {};
            circuit.junctionBoxes.insert(pair.box1);
            circuit.junctionBoxes.insert(pair.box2);
            circuit.circuitDistance += pair.distance;
            circuits.push_back(circuit);
            lastMergePair = pair;
            hasMerge = true;
        } else if (circuit1Index == circuit2Index && circuit1Index != -1) {
            circuits[circuit1Index].circuitDistance += pair.distance;
        } else if (circuit1Index != -1 && circuit2Index == -1) {
            circuits[circuit1Index].junctionBoxes.insert(pair.box2);
            circuits[circuit1Index].circuitDistance += pair.distance;
            lastMergePair = pair;
            hasMerge = true;
        } else if (circuit1Index == -1 && circuit2Index != -1) {
            circuits[circuit2Index].junctionBoxes.insert(pair.box1);
            circuits[circuit2Index].circuitDistance += pair.distance;
            lastMergePair = pair;
            hasMerge = true;
        } else {
            for (const JunctionBox& box : circuits[circuit2Index].junctionBoxes) {
                circuits[circuit1Index].junctionBoxes.insert(box);
            }
            circuits[circuit1Index].circuitDistance += circuits[circuit2Index].circuitDistance + pair.distance;
            circuits.erase(circuits.begin() + circuit2Index);
            lastMergePair = pair;
            hasMerge = true;
        }
    }

    if (hasMerge) {
        long long multi = static_cast<long long>(lastMergePair.box1.x) * lastMergePair.box2.x;
        std::cout << lastMergePair.box1.x << ", " << lastMergePair.box2.x << std::endl;
        std::cout << "Last pair X multiplication: " << multi << std::endl;
    }

    int connected = 0;
    int biggest = 0, bigger = 0, big = 0;
    for (const Circuit& circuit : circuits) {
        int circuitSize = circuit.junctionBoxes.size();
        if (circuitSize > biggest) {
            big = bigger;
            bigger = biggest;
            biggest = circuitSize;
        } else if (circuitSize > bigger) {
            big = bigger;
            bigger = circuitSize;
        } else if (circuitSize > big) {
            big = circuitSize;
        }
        connected += circuitSize;
    }

    std::cout << "Not connected: " << map.size() - connected << std::endl;
    std::cout << "Biggest: " << biggest << std::endl;
    std::cout << "Bigger: " << bigger << std::endl;
    std::cout << "Big: " << big << std::endl;
    long long multi = biggest * bigger * big;
    std::cout << "Multiplicated: " << multi << std::endl;
}

void partTwo() {

}

double calculateDistance(JunctionBox junctionBox1, JunctionBox junctionBox2) {
    return std::sqrt(std::pow(junctionBox1.x - junctionBox2.x, 2) + std::pow(junctionBox1.y - junctionBox2.y, 2) + std::pow(junctionBox1.z - junctionBox2.z, 2));
}