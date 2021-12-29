#include "board.hpp"

#include <utility>

bool Board::callNumber(int number) {
  if (_hasWon) {
    return false;
  }
  for (auto &row: _board) {
    for (auto &num: row) {
      if (num == number) {
        num = 0;
        break;
      }
    }
  }
  return checkWin();
}

bool Board::checkWin() {
  for (auto i{0}; i < _board.size(); i++) {
    int rowSum{0}, columnSum{0};
    for (auto ii{0}; ii < _board[i].size(); ii++) {
      rowSum += _board[i][ii];
      columnSum += _board[ii][i];
    }
    if (rowSum == 0 || columnSum == 0) {
      _hasWon = true;
      return true;
    }
  }
  return false;
}

int Board::getScore(int number) {
  int sum{0};
  for (const auto& row: _board) {
    for (auto num: row) {
      sum += num;
    }
  }
  return sum * number;
}
Board::Board(std::vector<std::vector<int>> board) : _board{std::move(board)} {
}
