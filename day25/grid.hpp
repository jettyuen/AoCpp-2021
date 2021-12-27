#pragma once
#include <iostream>
#include <utility>
#include <vector>

enum class CellState { Empty, East, South };

class Grid {
  public:
  int run();
  explicit Grid(std::vector<std::vector<CellState>> cells)
      : _gridMap{std::move(cells)} {
  }
  void printMap();

  private:
  std::vector<std::vector<CellState>> _gridMap;

  bool step(CellState state);
};