#include <fstream>
#include <vector>
#include <iostream>
#include <set>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>

struct Tile {
    int posX;
    int posY;
};

std::set<std::pair<int, int>> valid;

void partOne(const std::vector<Tile>& map);
void partTwo(std::vector<Tile>& map);
long long calculateArea(Tile one, Tile two);

int main() {
    // std::fstream file("test-input-9");
    std::fstream file("input-9");

    std::string line;
    std::vector<Tile> map;

    while (getline(file, line)) {
        std::stringstream ss(line);
        Tile tile;
        bool start = true;
        std::string number;
        while (getline(ss, number, ',')) {
            if (start) {
                tile.posX = std::stoi(number);
            } else {
                tile.posY = std::stoi(number);
            }
            start = false;
        }
        map.push_back(tile);
    }

    partOne(map);

    partTwo(map);
    return 0;
}

void partOne(const std::vector<Tile>& map) {
    long long maxArea = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = i + 1; j < map.size(); j++) {
            maxArea = std::max(maxArea, calculateArea(map[i], map[j]));
        }
    }
    std::cout << "The largest possible area is " << maxArea << std::endl;
}

long long calculateArea(Tile one, Tile two) {
    int sideA;
    int sideB;
    if (one.posX > two.posX) {
        sideA = one.posX - two.posX + 1;
    } else {
        sideA = two.posX - one.posX + 1;
    }

    if (one.posY > two.posY) {
        sideB = one.posY - two.posY + 1;
    } else {
        sideB = two.posY - one.posY + 1;
    }
    return static_cast<long long>(sideA) * sideB;
}

bool isPointInPolygon(const std::vector<Tile>& polygon, int x, int y) {
    int n = polygon.size();
    bool inside = false;

    for (int i = 0, j = n - 1; i < n; j = i++) {
        int xi = polygon[i].posX, yi = polygon[i].posY;
        int xj = polygon[j].posX, yj = polygon[j].posY;

        bool intersect = ((yi > y) != (yj > y)) &&
                        (x < (xj - xi) * (y - yi) / (yj - yi) + xi);
        if (intersect) inside = !inside;
    }

    return inside;
}

bool isPointOnEdge(const std::vector<Tile>& polygon, int x, int y) {
    int n = polygon.size();

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int x1 = polygon[i].posX, y1 = polygon[i].posY;
        int x2 = polygon[j].posX, y2 = polygon[j].posY;

        if (x1 == x2 && x == x1) {
            if ((y >= std::min(y1, y2)) && (y <= std::max(y1, y2))) {
                return true;
            }
        } else if (y1 == y2 && y == y1) {
            if ((x >= std::min(x1, x2)) && (x <= std::max(x1, x2))) {
                return true;
            }
        }
    }

    return false;
}

bool checkTiles(const std::vector<Tile>& map, Tile tile1, Tile tile2) {
    int startCol = std::min(tile1.posX, tile2.posX);
    int endCol = std::max(tile1.posX, tile2.posX);
    int startRow = std::min(tile1.posY, tile2.posY);
    int endRow = std::max(tile1.posY, tile2.posY);

    for (int i = startCol; i <= endCol; i++) {
        if (!isPointInPolygon(map, i, startRow) && !isPointOnEdge(map, i, startRow)) {
            return false;
        }
        if (!isPointInPolygon(map, i, endRow) && !isPointOnEdge(map, i, endRow)) {
            return false;
        }
    }

    for (int i = startRow; i <= endRow; i++) {
        if (!isPointInPolygon(map, startCol, i) && !isPointOnEdge(map, startCol, i)) {
            return false;
        }
        if (!isPointInPolygon(map, endCol, i) && !isPointOnEdge(map, endCol, i)) {
            return false;
        }
    }

    return true;
}

void partTwoWorker(const std::vector<Tile>& map, int startIdx, int endIdx,
                   std::atomic<long long>& maxArea, std::mutex& mtx) {
    long long localMax = 0;

    for (int i = startIdx; i < endIdx && i < map.size(); i++) {
        for (int j = i + 1; j < map.size(); j++) {
            long long area = calculateArea(map[i], map[j]);

            long long currentMax = maxArea.load(std::memory_order_relaxed);
            if (area > currentMax) {
                if (checkTiles(map, map[i], map[j])) {
                    localMax = std::max(localMax, area);

                    long long expected = maxArea.load(std::memory_order_relaxed);
                    while (localMax > expected &&
                           !maxArea.compare_exchange_weak(expected, localMax,
                                                          std::memory_order_relaxed)) {
                    }
                }
            }
        }
    }
}

void partTwo(std::vector<Tile>& map) {
    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "Using " << numThreads << " threads" << std::endl;

    std::vector<std::thread> threads;
    std::atomic<long long> maxArea(0);
    std::mutex mtx;

    int pointsPerThread = (map.size() + numThreads - 1) / numThreads;
    std::vector<std::pair<int, int>> tiles;

    for (unsigned int t = 0; t < numThreads; t++) {
        int startIdx = t * pointsPerThread;
        int endIdx = std::min((int)((t + 1) * pointsPerThread), (int)map.size());

        threads.emplace_back(partTwoWorker, std::ref(map), startIdx, endIdx,
                            std::ref(maxArea), std::ref(mtx));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "The largest possible area with green Tiles is "
              << maxArea.load() << std::endl;
}