#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
// Processs strings and return a
enum class InstructionType { Forward, Down, Up };

struct Instruction {
  InstructionType direction;
  int distance;
};

std::vector<Instruction> parseInstructions(const std::string &fileName);
std::vector<std::vector<int>>
storeInstructions(const std::vector<Instruction> &instructions);
std::tuple<int, int>
runInstructions(const std::vector<std::vector<int>> &instructionsByType);
int getPosByDist(const std::vector<Instruction> &instructions);

int main(int argc, char **argv) {
  std::vector<Instruction> instructions{};
  if (argc > 0) {
    instructions = parseInstructions(argv[1]);
    int posByDist = getPosByDist(instructions);
    std::cout << "Position * Distance: " << posByDist << std::endl;
  }
  return 0;
}

std::vector<Instruction> parseInstructions(const std::string &fileName) {
  std::vector<Instruction> instructions{};
  std::ifstream ifs(fileName);
  std::string line{};

  while (std::getline(ifs, line)) {
    Instruction instruction{};
    std::istringstream iss(line);
    std::string instrType{};
    if (std::getline(iss, instrType, ' ')) {
      if (instrType == "forward") {
        instruction.direction = InstructionType::Forward;
      } else if (instrType == "down") {
        instruction.direction = InstructionType::Down;
      } else if (instrType == "up") {
        instruction.direction = InstructionType::Up;
      } else {
        std::cerr << "Incorrect Instruction Type!" << std::endl;
        break;
      }
      std::string dist{};
      if (std::getline(iss, dist)) {
        instruction.distance = std::stoi(dist);
        instructions.push_back(instruction);
      }
    }
  }
  ifs.close();
  return instructions;
}

std::vector<std::vector<int>>
storeInstructions(const std::vector<Instruction> &instructions) {
  int aim{};
  std::vector<std::vector<int>> posDistData(2);
  for (auto &instruction: instructions) {
    if (instruction.direction == InstructionType::Forward) {
      posDistData.at(0).push_back(instruction.distance);
      posDistData.at(1).push_back(aim * instruction.distance);
    } else if (instruction.direction == InstructionType::Down) {
      aim += instruction.distance;
    } else if (instruction.direction == InstructionType::Up) {
      aim -= instruction.distance;
    }
  }
  return posDistData;
}

std::tuple<int, int>
runInstructions(const std::vector<std::vector<int>> &instructionsByType) {
  int pos{};
  int depth{};
  for (auto &posInstruction: instructionsByType.at(0)) {
    pos += posInstruction;
  }
  for (auto &depthInstruction: instructionsByType.at(1)) {
    depth += depthInstruction;
  }
  return std::make_tuple(pos, depth);
}

int getPosByDist(const std::vector<Instruction> &instructions) {
  std::vector<std::vector<int>> storedInstructions =
      storeInstructions(instructions);
  std::tuple<int, int> posAndDist = runInstructions(storedInstructions);
  std::cout << "Position: " << std::get<0>(posAndDist) << std::endl;
  std::cout << "Depth: " << std::get<1>(posAndDist) << std::endl;
  return std::get<0>(posAndDist) * std::get<1>(posAndDist);
}