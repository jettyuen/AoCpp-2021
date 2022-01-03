#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

void getInput(const std::string &fileName, std::vector<int> &positions);
int getFuel(std::vector<int> &positions);
int getMedian(std::vector<int> &positions);
int getTriangularFuel(std::vector<int> &positions);

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<int> positions{};
  getInput(argv[1], positions);
  std::cout << "Fuel cost (Linear): " << getFuel(positions) << std::endl;
  std::cout << "Fuel cost (Triangular): " << getTriangularFuel(positions)
            << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Program took " << duration.count() << " milliseconds"
            << std::endl;
  return 0;
}

void getInput(const std::string &fileName, std::vector<int> &positions) {
  std::ifstream ifs{fileName};
  std::string line{};
  std::getline(ifs, line);
  std::istringstream iss{line};
  std::string numStr{};
  std::vector<int> nums{};
  while (iss.good()) {
    char c = iss.get();
    if (c == ',') {
      positions.push_back(std::stoi(numStr));
      numStr.clear();
      continue;
    }
    numStr.push_back(c);
  }
  ifs.close();
}
int getFuel(std::vector<int> &positions) {
  int median = getMedian(positions);
  int fuelSum{};
  for (auto &pos: positions) {
    fuelSum += abs(pos - median);
  }
  return fuelSum;
}
int getMedian(std::vector<int> &positions) {
  std::sort(positions.begin(), positions.end());
  return positions[positions.size() / 2];
}
int getTriangularFuel(std::vector<int> &positions) {
  std::sort(positions.begin(), positions.end());
  std::map<int, int> distMap{};
  int min{positions[0]}, max{positions[positions.size() - 1]};
  for (int i{min}; i <= max; i++) {
    int distSum{};
    for (auto num: positions) {
      int dist = abs(num - i);
      distSum += ((dist * dist) + dist) / 2;
    }
    distMap.emplace(distSum, i);
  }
  return distMap.begin()->first;
}
