#include <algorithm>
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

  int getRiskLevel() {
    std::vector<int> lowPoints = getLowPoints();
    int sum{0};
    for (auto &lowPoint: lowPoints) {
      sum += _nodeMap[lowPoint].height + 1;
    }
    return sum;
  }
  int getLargestBasins() {
    std::vector<std::set<int>> basins = getBasins(getLowPoints());
    std::sort(
      basins.begin(), basins.end(),
      [](const auto &lhs, const auto &rhs) { return lhs.size() > rhs.size(); });
    int basin1 = (int) basins[0].size();
    int basin2 = (int) basins[1].size();
    int basin3 = (int) basins[2].size();
    return basin1 * basin2 * basin3;
  }

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
  void dfsRemove(const int &nodeVal, std::set<int> &searched) {
    Node node = _nodeMap[nodeVal];
    for (auto &neighbor: node.neighbors) {
      if (_nodeMap[neighbor].height > node.height) {
        searched.emplace(neighbor);
        dfsRemove(neighbor, searched);
      }
    }
  }
  void dfsAdd(const int &nodeVal, std::set<int> &basin) {
    Node node = _nodeMap[nodeVal];
    for (auto &neighbor: node.neighbors) {
      if (_nodeMap[neighbor].height != 9 && !basin.contains(neighbor)) {
        basin.emplace(neighbor);
        dfsAdd(neighbor, basin);
      }
    }
  }
  std::vector<int> getLowPoints() {
    std::vector<int> lowPoints{};
    std::set<int> searched{};
    for (auto &keyValue: _nodeMap) {
      if (searched.contains(keyValue.first)) {
        continue;
      }
      Node node = keyValue.second;
      bool isLowPoint{false};
      for (auto &neighbor: node.neighbors) {
        if (_nodeMap[neighbor].height <= node.height) {
          isLowPoint = false;

          break;
        } else {
          isLowPoint = true;
        }
      }
      if (isLowPoint) {
        lowPoints.push_back(keyValue.first);
        dfsRemove(keyValue.first, searched);
      }
    }
    return lowPoints;
  }
  std::vector<std::set<int>> getBasins(const std::vector<int> &lowPoints) {
    std::vector<std::set<int>> basins{};
    for (auto i{0}; i < lowPoints.size(); i++) {
      basins.emplace_back();
      dfsAdd(lowPoints[i], basins[i]);
    }
    return basins;
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
        edges.emplace_back(x, i + (y * row.size()));
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
    int num;
    nums.emplace_back();
    for (auto &c: line) {
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
  std::cout << "Risk Level: " << graph.getRiskLevel() << std::endl;
  std::cout << "Basin value: " << graph.getLargestBasins() << std::endl;
  return 0;
}