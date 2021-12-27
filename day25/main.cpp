#include "grid.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> getInput(const std::string &fileName);
std::vector<std::vector<CellState>> createGrid(const std::vector<std::string>
    &lines);

int main(int argc, char **argv) {
  if (argc < 1) { return -1; }
  std::vector<std::string> lines = getInput(argv[1]);
  std::vector<std::vector<CellState>> gridMap = createGrid(lines);
  Grid grid{gridMap};
  int stepCount = grid.run();
  std::cout << "Steps: " << stepCount << std::endl;
  grid.printMap();
  return 0;
}

std::vector<std::string> getInput(const std::string &fileName) {
  std::ifstream ifs{fileName};
  std::string line{};
  std::vector<std::string> lines{};
  while (std::getline(ifs, line)) { lines.push_back(line); }
  ifs.close();
  return lines;
}

std::vector<std::vector<CellState>> createGrid(const std::vector<std::string>
    &lines) {
  const std::size_t lineCount{lines.size()};
  std::vector<std::vector<CellState>> gridMap{lineCount};
  for (auto i{0}; i < lineCount; i++) {
    for (auto &c: lines.at(i)) {
      switch (c) {
        case '.':
          gridMap[i].push_back(CellState::Empty);
          break;
        case '>':
          gridMap[i].push_back(CellState::East);
          break;
        case 'v':
          gridMap[i].push_back(CellState::South);
          break;
        default:
          std::cerr << "Error: Input contains invalid character(s)."
                    << std::endl;
          break;
      }
    }
  }
  return gridMap;
}