#include <fstream>
#include <iostream>
#include <map>
#include <set>

// std::pair<char, char> pair;
//

/* Store all letters of initial polymer into a map of long long by char */
/* Parse input, and store pairs into a map of count by Polymer pair. */ /* When stepping through, we just need to check if rule exists, then if map * has rule, deduct rule pair count, and increase pair for left + insertion
 * and insertion + right. If it does not exist yet, add to map. otherwise,
 * simply increment key value pair.*/

void getInput(const std::string &fileName,
              std::map<std::pair<char, char>, char> &rules,
              std::map<std::pair<char, char>, long long> &pairs,
              std::map<char, long long> &elementCount) {
  std::ifstream ifs{fileName};
  std::string polymer{}, line{};
  std::getline(ifs, polymer);
  for (const auto &c: polymer) {
    if (elementCount.contains(c)) {
      elementCount[c]++;
    } else {
      elementCount.emplace(c, 1);
    }
  }
  for (auto i = 0; i < polymer.length() - 1; i++) {
    std::pair<char, char> pair{polymer[i], polymer[i + 1]};
    if (pairs.contains(pair)) {
      pairs[pair]++;
    } else {
      pairs.emplace(pair, 1);
    }
  }
  while (std::getline(ifs, line)) {
    if (line.empty()) {
      continue;
    }
    const std::string delim{" -> "};
    auto next = line.find(delim);
    std::pair<char, char> pair{line[0], line[next - 1]};
    char insert{line[next + delim.length()]};
    rules.emplace(pair, insert);
  }
}

long long polymerize(int steps, std::map<std::pair<char, char>, char> &rules,
                     std::map<std::pair<char, char>, long long> &pairs,
                     std::map<char, long long> &elementCount) {
  std::map<std::pair<char, char>, long long> changesThisStep{};
  while (steps-- > 0) {
    for (auto &pair: pairs) {
      if (rules.contains(pair.first)) {
        if (elementCount.contains(rules[pair.first])) {
          elementCount[rules[pair.first]] += pair.second;
        } else {
          elementCount.emplace(rules[pair.first], pair.second);
        }
        std::pair<char, char> left{pair.first.first, rules[pair.first]};
        std::pair<char, char> right{rules[pair.first], pair.first.second};
        if (changesThisStep.contains(left)) {
          changesThisStep[left] += pair.second;
        } else {
          changesThisStep.emplace(left, pair.second);
        }
        if (changesThisStep.contains(right)) {
          changesThisStep[right] += pair.second;
        } else {
          changesThisStep.emplace(right, pair.second);
        }
        if (changesThisStep.contains(pair.first)) {
          changesThisStep[pair.first] -= pair.second;
        } else {
          changesThisStep.emplace(pair.first, -pair.second);
        }
      }
    }
    // Every round, do this.
    for (auto &change: changesThisStep) {
      if (pairs.contains(change.first)) {
        pairs[change.first] += change.second;
      } else {
        pairs.emplace(change.first, change.second);
      }
      change.second = 0;
    }
  }
  long long max = 0;
  long long min = LLONG_MAX;
  for (auto &element: elementCount) {
    if (element.second > max) {
      max = element.second;
    }
    if (element.second < min) {
      min = element.second;
    }
  }
  return max - min;
}

int main(int argc, char **argv) {
  auto start = std::chrono::high_resolution_clock::now();
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  std::map<std::pair<char, char>, char> rules{};
  std::map<std::pair<char, char>, long long> pairs{};
  std::map<char, long long> elementCount{};
  getInput(argv[1], rules, pairs, elementCount);
  long long polyResult = polymerize(40, rules, pairs, elementCount);
  std::cout << "After 40 steps: " << polyResult << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Program took " << duration.count() << " milliseconds"
            << std::endl;
  return 0;
}