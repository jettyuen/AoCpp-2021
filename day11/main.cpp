#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <chrono>
#define FLASH_THRESH 9

class Graph {
  public:
  Graph(std::map<int, int> &nodes, std::set<std::pair<int, int>> &edges)
      : _edges{edges}, _nodes{nodes} {
    buildNodes();
  }
  struct Node {
    int value;
    std::set<int> neighbors;
  };

  int simulate(int steps) {
    int flashCount{};
    while (steps-- > 0) {
      for (auto &node: _nodes) {
        node.second++;
      }
      for (auto &node: _nodes) {
        if (node.second > FLASH_THRESH) {
          node.second = 0;
          flashCount++;
          dfsIncrement(node.first, flashCount);
        }
      }
    }
    return flashCount;
  }
  int simulateUntilSync() {
    bool foundSync{};
    int flashCount{}, steps{};
    while (!foundSync) {
      steps++;
      for (auto &node: _nodes) {
        node.second++;
      }
      int flashDelta{flashCount};
      for (auto &node: _nodes) {
        if (node.second > FLASH_THRESH) {
          node.second = 0;
          flashCount++;
          dfsIncrement(node.first, flashCount);
        }
      }
      flashDelta = flashCount - flashDelta;
      if (flashDelta == _nodes.size()) {
        foundSync = true;
      }
    }
    return steps;
  }

  private:
  std::map<int, Node> _nodeMap;
  std::map<int, int> _nodes;
  std::set<std::pair<int, int>> _edges;

  void buildNodes() {
    for (auto &edge: _edges) {
      addNeighbor(edge.first, edge.second);
      addNeighbor(edge.second, edge.first);
    }
  }

  void addNeighbor(int n1, int n2) {
    if (_nodeMap.find(n1) == _nodeMap.end()) {
      Node node{_nodes[n1], {n2}};
      _nodeMap.emplace(n1, node);
    } else {
      _nodeMap[n1].neighbors.emplace(n2);
    }
  }
  void dfsIncrement(const int &nodeVal, int &flashCount) {
    Node node = _nodeMap[nodeVal];
    for (auto &neighbor: node.neighbors) {
      if (_nodes[neighbor] != 0) {
        _nodes[neighbor]++;
        if (_nodes[neighbor] > FLASH_THRESH) {
          _nodes[neighbor] = 0;
          flashCount++;
          dfsIncrement(neighbor, flashCount);
        }
      }
    }
  }
};

void getInput(const std::string &fileName,
              std::vector<std::vector<int>> &nums) {
  std::ifstream ifs{fileName};
  std::string line{};
  while (std::getline(ifs, line)) {
    std::vector<int> numbers{};
    for (auto &c: line) {
      numbers.push_back(c - '0');
    }
    nums.emplace_back(numbers);
  }
}
void processInput(const std::vector<std::vector<int>> &nums,
                  std::map<int, int> &nodes,
                  std::set<std::pair<int, int>> &edges) {
  int rowLength = (int) nums[0].size();
  int rows      = (int) nums.size();
  for (auto i{0}; i < rows; i++) {
    for (auto ii{0}; ii < rowLength; ii++) {
      int x = i * rowLength + ii;
      nodes.emplace(x, nums[i][ii]);
      if (ii < rowLength - 1) {// before last element in low
        edges.emplace(x, x + 1);
        if (i - 1 >= 0) {
          edges.emplace(x, x - rowLength + 1);
        }
      }
      if (i + 1 < rows) {// before last row
        edges.emplace(x, x + rowLength);
        if (ii < rowLength - 1) {
          edges.emplace(x, x + rowLength + 1);
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  auto start = std::chrono::high_resolution_clock::now();
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  std::vector<std::vector<int>> nums;
  std::set<std::pair<int, int>> edges;
  std::map<int, int> nodes;
  getInput(argv[1], nums);
  processInput(nums, nodes, edges);
  Graph graph{nodes, edges};
  Graph graph1{graph};
  int flashCount = graph.simulate(100);
  int steps      = graph1.simulateUntilSync();
  std::cout << "Flash Count: " << flashCount << std::endl;
  std::cout << "All octopuses flashed on step " << steps << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Program took " << duration.count() << " milliseconds"
            << std::endl;
  return 0;
}