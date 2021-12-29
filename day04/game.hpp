#pragma once
#include "board.hpp"
#include <queue>

class Game {
  public:
  void run();
  Game(std::queue<int> &numbers, std::vector<Board> &boards);

  private:
  std::queue<int> _numbers;
  std::vector<Board> _boards;
};