#include "game.hpp"
#include <fstream>
#include <iostream>

void getInput(const std::string &fileName, std::queue<int> &numbers,
              std::vector<Board> &boards);
int main(int argc, char **argv) {
  if (argc == 1) {
    return -1;
  }
  std::queue<int> numbers{};
  std::vector<Board> boards{};
  getInput(argv[1], numbers, boards);
  Game game{numbers, boards};
  game.run();
  return 0;
}

void getInput(const std::string &fileName, std::queue<int> &numbers,
              std::vector<Board> &boards) {
  std::ifstream ifs{fileName};
  std::string line{};

  size_t pos{};
  std::getline(ifs, line);
  while (true) {
    pos = line.find(',');
    int num = std::stoi(line.substr(0, pos));
    numbers.push(num);
    line.erase(0, pos + 1);
    if (pos == std::string::npos) {
      break;
    }
  }

  std::getline(ifs, line);
  auto index{0};
  std::string numStr{};
  std::vector<std::vector<int>> boardNumbers{5};
  while (std::getline(ifs, line)) {
    if (index == 0) {

      std::vector<std::vector<int>> newBoard{5};
      boardNumbers = newBoard;
    }
    if (line.empty()) {
      Board board{boardNumbers};
      boards.push_back(board);
      index = 0;
      continue;
    }
    while (true) {
      pos = line.find(' ');
      numStr =  line.substr(0, pos);
      if(numStr.empty()) {
        if (pos == std::string::npos) {
          break;
        }
        line.erase(0, 1);
        continue;
      }
      int num = std::stoi(numStr);
      boardNumbers[index].push_back(num);
      line.erase(0, pos + 1);
      if (pos == std::string::npos) {
        break;
      }
    }
    index++;
  }

  ifs.close();
}
