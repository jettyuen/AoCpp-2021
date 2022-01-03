#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

// Sort input nums by length, storing the output nums for later decoding.
// Each line represents input to find code, and an output to decode.

// 1. Find 1(c/f),
// 2. 7(*(a), c/f),
// 3. 4(a, c/f, b/d
// 4. 2(a, *(b), c, *(d), *(e), *(f)) -- Contains either b/d, and either c/f
// 5. 9(a, b, c, d, e, f, *(g)), -- Contains a->f
// 6. 8 (a, b, c, d, *(e), f, g)

void splitAtDelim(const std::string &str, const std::string &delim,
                  std::pair<std::string, std::string> &pair);
void getKey(std::vector<std::string> &input, std::map<char, char> &keyMap);
void getCount(const std::vector<std::string> &output,
              std::map<char, char> &keyMap,
              std::map<std::string, int> &valueMap,
              std::map<int, int> &countMap);

int main(int argc, char **argv) {
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
  while (std::getline(ifs, line)) {
    splitAtDelim(line, " | ", ioPair);
    std::cout << ioPair.first << " | " << ioPair.second << std::endl;
    std::istringstream issI{ioPair.first};
    std::string wordI{};
    std::vector<std::string> ssI{};
    while (issI) {
      char c = issI.get();
      if (c == ' ') {
        ssI.push_back(wordI);
        wordI.clear();
        continue;
      }
      wordI.push_back(c);
    }
    getKey(ssI, keyMap);
    std::vector<std::string> ssO{};
    std::string wordO{};
    ioPair.second.push_back(' ');
    std::istringstream issO{ioPair.second};
    while (issO) {
      char c = issO.get();
      if (c == ' ') {
        ssO.push_back(wordO);
        wordO.clear();
        continue;
      }
      wordO.push_back(c);
    }
    getCount(ssO, keyMap, valueMap, countMap);
  }

  int uniques{};
  for (auto &k: countMap) {
    auto first = k.first;
    std::cout << first << ": " << k.second << std::endl;
    if (first == 1 || first == 4 || first == 7 || first == 8) {
      uniques += k.second;
    }
  }
  std::cout << "Uniques: " << uniques << std::endl;
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
      std::cout << "1(c/f): " << s << std::endl;
      cOrF.first = s[0];
      cOrF.second = s[1];
    } else if (s.length() == 3) {// 7
      std::cout << "7(a): " << s << std::endl;
      for (auto ch: s) {
        if (ch != cOrF.first && ch != cOrF.second) {
          a = ch;
          keyMap[a] = 'a';
        }
      }
    } else if (s.length() == 4) {// 4
      std::cout << "4(b/d): " << s << std::endl;
      for (auto ch: s) {
        if (ch != cOrF.first && ch != cOrF.second) {
          if (bOrD.first == '\0') {
            bOrD.first = ch;
          } else {
            bOrD.second = ch;
          }
        }
      }
    } else if (s.length() == 5) {// 2
      std::cout << "2(b, c, d, f): " << s << std::endl;
      int cFCount{}, bDCount{};
      for (auto &ch: s) {
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
        std::cout << a << ',' << b << ',' << c << ',' << d << ',' << f
                  << std::endl;
        keyMap[b] = 'b';
        keyMap[c] = 'c';
        keyMap[d] = 'd';
        keyMap[f] = 'f';
      }
    } else if (s.length() == 6) {// 9
      std::cout << "9(g): " << s << std::endl;
      int satisfiedCount{};
      char possibleG{};
      for (auto ch: s) {
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
      std::cout << "8(e): " << s << std::endl;
      std::cout << keyMap[a] << ", " << a << std::endl;
      std::cout << keyMap[b] << ", " << b << std::endl;
      std::cout << keyMap[c] << ", " << c << std::endl;
      std::cout << keyMap[d] << ", " << d << std::endl;
      for (auto ch: s) {
        if (ch != a && ch != b && ch != c && ch != d && ch != f && ch != g) {
          e = ch;
          keyMap[e] = 'e';
          std::cout << keyMap[e] << ", " << e << std::endl;
        }
      }
      std::cout << keyMap[f] << ", " << f << std::endl;
      std::cout << keyMap[g] << ", " << g << std::endl;
      std::cout << std::endl;
    }
  }
}

void getCount(const std::vector<std::string> &output,
              std::map<char, char> &keyMap,
              std::map<std::string, int> &valueMap,
              std::map<int, int> &countMap) {
  for (const std::string &s: output) {
    std::string ts{s};
    for (auto &c: ts) {
      c = keyMap[c];
    }
    std::cout << s << std::endl;
    std::sort(ts.begin(), ts.end());
    std::cout << ts << std::endl;
    if (valueMap.count(ts) == 1) {
      int i = valueMap[ts];
      countMap[i]++;
    }
  }
}
