#include "grid.hpp"

int Grid::run() {
  int steps{0};
  bool eastStepped{true}, southStepped{true};
  while (eastStepped || southStepped) {
    eastStepped = step(CellState::East);
    southStepped = step(CellState::South);
    steps++;
  }
  return steps;
}

bool Grid::step(const CellState state) {
  // make copies before step so overflow herds are handled
  std::vector<std::vector<CellState>> beforeStepMap{_gridMap};
  bool hasStepped{};
  for (auto row{0}; row < beforeStepMap.size(); row++) {
    for (auto col{0}; col < beforeStepMap[0].size(); col++) {
      if (beforeStepMap[row][col] == state) {
        int nextCoord{};
        if (state == CellState::East) {
          nextCoord = col == beforeStepMap[0].size() - 1 ? 0 : col + 1;
        } else {
          nextCoord = row == beforeStepMap.size() - 1 ? 0 : row + 1;
        }
        CellState &nextState = state == CellState::East
                                   ? beforeStepMap[row][nextCoord]
                                   : beforeStepMap[nextCoord][col];
        CellState &nextDest = state == CellState::East
                                  ? _gridMap[row][nextCoord]
                                  : _gridMap[nextCoord][col];
        if (nextState == CellState::Empty) {
          _gridMap[row][col] = CellState::Empty;
          nextDest = state;
          hasStepped = true;
        }
      }
    }
  }
  return hasStepped;
}
void Grid::printMap() {
  for (auto &row: _gridMap) {
    for (auto &col: row) {
      if (col == CellState::Empty) {
        std::cout << '.';
      } else if (col == CellState::East) {
        std::cout << '>';
      } else if (col == CellState::South) {
        std::cout << 'v';
      }
    }
    std::cout << std::endl;
  }
}