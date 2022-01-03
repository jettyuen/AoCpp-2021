#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

void splitAtDelim(const std::string &str, const std::string &delim,
                  std::pair<std::string, std::string> &pair);
void getKey(std::vector<std::string> &input, std::map<char, char> &keyMap);
void getCount(const std::vector<std::string> &output,
              std::map<char, char> &keyMap,
              std::map<std::string, int> &valueMap,
              std::map<int, int> &countMap, int &outSum);

int main(int argc, char **argv) {
  auto start = std::chrono::high_resolution_clock::now();
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }

  std::map<std::string, int> valueMap{
    {"abcefg", 0}, {"cf", 1},     {"acdeg", 2}, {"acdfg", 3},   {"bcdf", 4},
    {"abdfg", 5},  {"abdefg", 6}, {"acf", 7},   {"abcdefg", 8}, {"abcdfg", 9}};

  std::map<char, char> keyMap = {{'a', '\0'}, {'b', '\0'}, {'c', '\0'},
                                 {'d', '\0'}, {'e', '\0'}, {'f', '\0'},
                                 {'g', '\0'}};

  std::map<int, int> countMap = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},
                                 {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}};
  std::string line{};
  std::ifstream ifs{argv[1]};
  std::pair<std::string, std::string> ioPair = std::make_pair("", "");
  int outSum{0};
  while (std::getline(ifs, line)) {
    splitAtDelim(line, " | ", ioPair);
    std::istringstream issI{ioPair.first};
    std::string word{};
    std::vector<std::string> ss{};
    while (issI) {
      char c = issI.get();
      if (c == ' ') {
        ss.push_back(word);
        word.clear();
        continue;
      }
      word.push_back(c);
    }
    getKey(ss, keyMap);
    ss.clear();
    ioPair.second.push_back(' ');
    word.clear();
    std::istringstream issO{ioPair.second};
    while (issO) {
      char c = issO.get();
      if (c == ' ') {
        ss.push_back(word);
        word.clear();
        continue;
      }
      word.push_back(c);
    }
    getCount(ss, keyMap, valueMap, countMap, outSum);
  }

  int uniques{};
  for (auto &k: countMap) {
    auto first = k.first;
    if (first == 1 || first == 4 || first == 7 || first == 8) {
      uniques += k.second;
    }
  }

  std::cout << "Uniques: " << uniques << std::endl;
  std::cout << "Output Sum: " << outSum << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Program took " << duration.count() << " milliseconds"
            << std::endl;
  ifs.close();
  return 0;
}
void splitAtDelim(const std::string &str, const std::string &delim,
                  std::pair<std::string, std::string> &pair) {
  size_t delimPos = str.find(delim);
  std::string str1 = str.substr(0, delimPos + 1);
  std::string str2 = str.substr(delimPos + delim.length());
  pair.first = str1;
  pair.second = str2;
}

void getKey(std::vector<std::string> &input, std::map<char, char> &keyMap) {
  std::sort(input.begin(), input.end(),
            [](auto &s1, auto &s2) { return s1.length() < s2.length(); });
  std::pair<char, char> cOrF = std::make_pair('\0', '\0');
  std::pair<char, char> bOrD = std::make_pair('\0', '\0');
  char a{'\0'}, b{'\0'}, c{'\0'}, d{'\0'}, f{'\0'}, g{'\0'}, e{'\0'};
  for (const auto &s: input) {
    if (s.length() == 2) {// 1
      cOrF.first = s[0];
      cOrF.second = s[1];
    } else if (s.length() == 3) {// 7
      for (const auto &ch: s) {
        if (ch != cOrF.first && ch != cOrF.second) {
          a = ch;
          keyMap[a] = 'a';
        }
      }
    } else if (s.length() == 4) {// 4
      for (const auto &ch: s) {
        if (ch != cOrF.first && ch != cOrF.second) {
          if (bOrD.first == '\0') {
            bOrD.first = ch;
          } else {
            bOrD.second = ch;
          }
        }
      }
    } else if (s.length() == 5) {// 2
      int cFCount{}, bDCount{};
      for (const auto &ch: s) {
        if (ch == cOrF.first) {
          c = cOrF.first;
          f = cOrF.second;
          cFCount++;
        } else if (ch == cOrF.second) {
          c = cOrF.second;
          f = cOrF.first;
          cFCount++;
        } else if (ch == bOrD.first) {
          d = bOrD.first;
          b = bOrD.second;
          bDCount++;
        } else if (ch == bOrD.second) {
          d = bOrD.second;
          b = bOrD.first;
          bDCount++;
        }
      }
      if (cFCount == 1 && bDCount == 1) {
        keyMap[b] = 'b';
        keyMap[c] = 'c';
        keyMap[d] = 'd';
        keyMap[f] = 'f';
      }
    } else if (s.length() == 6) {// 9
      int satisfiedCount{};
      char possibleG{};
      for (const auto &ch: s) {
        if (ch == a || ch == b || ch == c || ch == d || ch == f) {
          satisfiedCount++;
        } else {
          possibleG = ch;
        }
      }
      if (satisfiedCount == 5) {
        g = possibleG;
        keyMap[g] = 'g';
      }
    } else if (s.length() == 7) {// 8
      for (const auto &ch: s) {
        if (ch != a && ch != b && ch != c && ch != d && ch != f && ch != g) {
          e = ch;
          keyMap[e] = 'e';
        }
      }
    }
  }
}

void getCount(const std::vector<std::string> &output,
              std::map<char, char> &keyMap,
              std::map<std::string, int> &valueMap,
              std::map<int, int> &countMap, int &outSum) {
  int place{1000};
  for (const std::string &s: output) {
    std::string ts{s};
    for (auto &c: ts) {
      c = keyMap[c];
    }
    std::sort(ts.begin(), ts.end());
    if (valueMap.count(ts) == 1) {
      int n = valueMap[ts];
      outSum += place * n;
      countMap[n]++;
    }
    place /= 10;
  }
}