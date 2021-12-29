#include "game.hpp"
#include <iostream>

Game::Game(std::queue<int> &numbers, std::vector<Board> &boards)
    : _numbers{numbers}, _boards{boards} {
}

void Game::run() {
  bool hasWinner{};
  while (!_numbers.empty() || !hasWinner) {
    int number = _numbers.front();
    _numbers.pop();
    for (auto &board: _boards) {
      if (board.callNumber(number)) {
        int score = board.getScore(number);
        std::cout << "Score: " << score << std::endl;
        hasWinner = true;
      }
    }
  }
}