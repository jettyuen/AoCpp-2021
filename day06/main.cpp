#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

void getInput(const std::string &fileName,
              std::map<int, std::string> &lanternFish);
std::string simulate(int days, std::map<int, std::string> &lanternFish);
std::string addDigitString(const std::string &str1, const std::string &str2);

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  std::map<int, std::string> lanternFish{{0, "0"}, {1, "0"}, {2, "0"},
                                         {3, "0"}, {4, "0"}, {5, "0"},
                                         {6, "0"}, {7, "0"}, {8, "0"}};
  getInput(argv[1], lanternFish);
  auto start = std::chrono::high_resolution_clock::now();
  std::cout << "Population: " << simulate(256, lanternFish) << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Program took " << duration.count() << " milliseconds" <<
    std::endl;
  return 0;
}

std::string addDigitString(const std::string &str1, const std::string &str2) {
  std::vector<int> digits1{}, digits2{};
  for (auto c: str1) {
    digits1.push_back(c - '0');
  }
  for (auto c: str2) {
    digits2.push_back(c - '0');
  }
  int lastIndex1 = (int) digits1.size() - 1;
  int lastIndex2 = (int) digits2.size() - 1;
  std::vector<int> sum{};
  int carry{0};
  while (lastIndex1 >= 0 || lastIndex2 >= 0) {
    int placeSum;
    if (lastIndex1 < 0) {
      placeSum = carry + digits2[lastIndex2];
    } else if (lastIndex2 < 0) {
      placeSum = carry + digits1[lastIndex1];
    } else {
      placeSum = digits1[lastIndex1] + digits2[lastIndex2] + carry;
    }
    carry = placeSum >= 10 ? 1 : 0;
    placeSum = carry == 1 ? placeSum - 10 : placeSum;
    sum.push_back(placeSum);
    if (lastIndex2 == 0 && lastIndex1 == 0 && carry == 1) {
      sum.push_back(carry);
    }
    if (lastIndex1 > lastIndex2 && carry == 1 && lastIndex1 == 0) {
      sum.push_back(carry);
    }
    lastIndex1--;
    lastIndex2--;
  }
  std::string numStr{};
  for (auto num: sum) {
    char n = num + '0';
    numStr.push_back(n);
  }
  std::reverse(numStr.begin(), numStr.end());
  return numStr;
}

void getInput(const std::string &fileName,
              std::map<int, std::string> &lanternFish) {
  std::ifstream ifs{fileName};
  std::string line{};
  std::getline(ifs, line);
  std::istringstream iss{line};
  std::string numStr{};
  char c;
  while (iss.good()) {
    c = iss.get();
    if (c == ',') {
      int t = std::stoi(numStr);
      lanternFish[t] = addDigitString(lanternFish[t], "1");
      numStr.clear();
      continue;
    }
    numStr.push_back(c);
  }
  ifs.close();
}

std::string simulate(int days, std::map<int, std::string> &lanternFish) {
  std::string sum{"0"};
  while (days-- != 0) {
    const std::string parents = lanternFish[0];
    for (int t{0}; t < lanternFish.size() - 1; t++) {
      lanternFish[t] = lanternFish[t + 1];
    }
    lanternFish[6] = addDigitString(lanternFish[6], parents);
    lanternFish[8] = parents;
  }
  for (auto &pair: lanternFish) {
    sum = addDigitString(sum, pair.second);
  }
  return sum;
}