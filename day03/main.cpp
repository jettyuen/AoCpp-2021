#include "diagnostic.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> getInput(const std::string &fileName);

int main(int argc, char **argv) {
  if (argc == 1) { return 0; }
  std::vector<std::string> lines = getInput(argv[1]);
  Diagnostic diagnostic{lines};
  diagnostic.report();
  return 0;
}
std::vector<std::string> getInput(const std::string &fileName) {
  std::ifstream ifs{fileName};
  std::vector<std::string> lines{};
  std::string line{};
  while (std::getline(ifs, line)) { lines.push_back(line); }
  ifs.close();
  return lines;
}