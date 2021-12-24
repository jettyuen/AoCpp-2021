#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int countIncreasing(const std::vector<int> &numbers);
std::vector<int> processInput(std::ifstream &ifs, const std::string &fileName);
std::vector<int> getSumWindow(const std::vector<int> &numbers, int window);

int main (int argc, char **argv) {
  std::string fileName;
  if (argc > 1) {
    fileName = argv[1];
  }
  std::ifstream ifs;
  std::vector<int> numbers = processInput(ifs, fileName);
  int count = countIncreasing(numbers);
  std::cout << "Times Increasing: " << count << std::endl;
  std::vector<int> sumWindows = getSumWindow(numbers, 3);
  int sumWindowCount = countIncreasing(sumWindows);
  std::cout << "Times Increasing: " << sumWindowCount << std::endl;
  return 0;
}

int countIncreasing(const std::vector<int> &numbers) {
  int increasing = 0;
  auto it = std::begin(numbers);
  auto itNext = std::begin(numbers);
  const auto end = std::end(numbers);
  while (itNext != end) {
    if (*it++ < *++itNext)
      increasing++;
  }
  return increasing;
}
std::vector<int> getSumWindow(const std::vector<int> &numbers, const int window) {
  auto lowerIt = std::begin(numbers);
  auto upperIt = std::begin(numbers);
  std::advance(upperIt, window - 1);
  const auto end = std::end(numbers);
  std::vector<int> sumWindows;
  while (upperIt != end) {
    int sumWindow = 0;
    while (lowerIt <= upperIt) {
      sumWindow += *lowerIt++;
    }
    sumWindows.push_back(sumWindow);
    std::advance(lowerIt, (-window) + 1);
    upperIt++;
  }
  return sumWindows;
}
std::vector<int> processInput(std::ifstream &ifs, const std::string &fileName) {
  ifs.open(fileName);
  std::string line;
  std::vector<int> numbers;
  while (std::getline(ifs, line)) {
    int number = std::stoi(line);
    numbers.push_back(number);
  }
  ifs.close();
  return numbers;
}
