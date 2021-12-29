#pragma once
#include <string>
#include <vector>

class Board {
  public:
  explicit Board(std::vector<std::vector<int>> _board);
  bool callNumber(int number);
  int getScore(int number);

  private:
  std::vector<std::vector<int>> _board{};
  bool _hasWon{};
  bool checkWin();
};