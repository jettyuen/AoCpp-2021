#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
// Represent Graph as nodes/vertices and edges as adjacency lists.

class Graph {
  public:
  Graph(std::map<int, int> &nodes, std::vector<std::pair<int, int>> &edges)
      : _edges{edges}, _nodes{nodes} {
    buildNodes();
  }
  struct Node {
    int height;
    std::set<int> neighbors;
  };

  private:
  std::map<int, Node> _nodeMap;
  std::map<int, int> _nodes;
  std::vector<std::pair<int, int>> _edges;

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
};

void processData(const std::vector<std::vector<int>> &nums,
                 std::vector<std::pair<int, int>> &edges,
                 std::map<int, int> &nodes) {
  int x{0}, y{1}, i{0};
  for (auto &row: nums) {
    for (auto &num: row) {
      if (i < row.size() - 1) {
        edges.emplace_back(x, x + 1);
      }
      if (y < nums.size()) {
        edges.emplace_back(x, i + (y * 10));
      }
      nodes.emplace(x, num);
      i++;
      x++;
    }
    i = 0;
    y++;
  }
}

void getInput(const std::string &fileName,
              std::vector<std::vector<int>> &nums) {
  std::ifstream ifs{fileName};
  std::string line{};

  int i{0};
  while (std::getline(ifs, line)) {
    int num; nums.emplace_back(); for (auto &c: line) {
      num = c - '0';
      nums[i].push_back(num);
    }
    i++;
  }
  ifs.close();
}

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  std::vector<std::vector<int>> nums{};
  std::vector<std::pair<int, int>> edges{};
  std::map<int, int> hByN{};
  getInput(argv[1], nums);
  processData(nums, edges, hByN);
  Graph graph{hByN, edges};

  return 0;
}