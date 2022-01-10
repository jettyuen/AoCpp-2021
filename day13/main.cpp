#include <fstream>
#include <iostream>
#include <set>
#include <vector>
void getInput(const std::string &fileName,
              std::set<std::pair<int, int>> &coordinates,
              std::vector<std::pair<int, char>> &folds) {
  std::ifstream ifs{fileName};
  std::string line{};
  while (std::getline(ifs, line)) {
    if (line.empty()) {
      break;
    }
    size_t next;
    if ((next = line.find(',')) != std::string::npos) {
      auto xStr = line.substr(0, next);
      auto yStr = line.substr(next + 1);
      auto xNum = std::stoi(xStr);
      auto yNum = std::stoi(yStr);
      coordinates.emplace(xNum, yNum);
    }
  }
  while (std::getline(ifs, line)) {
    size_t next;
    if ((next = line.find("x=")) != std::string::npos) {
      auto numStr = line.substr(next + 2);
      auto num    = std::stoi(numStr);
      folds.emplace_back(num, 'x');
    } else if ((next = line.find("y=")) != std::string::npos) {
      auto numStr = line.substr(next + 2);
      auto num    = std::stoi(numStr);
      folds.emplace_back(num, 'y');
    }
  }
}
void fold(std::set<std::pair<int, int>> &coordinates,
          std::vector<std::pair<int, char>> &folds) {
  for (const auto &fold: folds) {
    if (fold.second == 'x') {
      std::set<std::pair<int, int>> removed{};
      for (const auto &coordinate: coordinates) {
        if (coordinate.first > fold.first) {
          auto oldCoord{coordinate};
          std::pair<int, int> newCoord{
            fold.first - (oldCoord.first - fold.first), oldCoord.second};
          if (!coordinates.contains(newCoord)) {
            coordinates.emplace(newCoord);
          }
          removed.emplace(oldCoord);
        }
      }
      for (auto &pair: removed) {
        coordinates.erase(pair);
      }
    } else {
      std::set<std::pair<int, int>> removed{};
      for (const auto &coordinate: coordinates) {
        if (coordinate.second > fold.first) {
          std::pair<int, int> oldCoord{coordinate};
          std::pair<int, int> newCoord = std::make_pair(
            oldCoord.first, fold.first - (oldCoord.second - fold.first));
          if (!coordinates.contains(newCoord)) {
            coordinates.emplace(newCoord);
          }
          removed.emplace(oldCoord);
        }
      }
      for (auto &pair: removed) {
        coordinates.erase(pair);
      }
    }
  }
}
void printInstructions(const std::set<std::pair<int, int>> &coordinates) {
  int xMax{0}, yMax{0};
  for (const auto &pair: coordinates) {
    if (pair.first > xMax) {
      xMax = pair.first;
    }
    if (pair.second > yMax) {
      yMax = pair.second;
    }
  }
  for (auto x{0}; x <= xMax; x++) {
    for (auto y{0}; y <= yMax; y++) {
      if (coordinates.contains({x, y})) {
        std::cout << '#';
      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
  std::cout << "x max: " << xMax << ", y max: " << yMax << std::endl;
}
int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  std::set<std::pair<int, int>> coordinates{};
  std::vector<std::pair<int, char>> folds{};
  getInput(argv[1], coordinates, folds);
  fold(coordinates, folds);
  printInstructions(coordinates);
  return 0;
}