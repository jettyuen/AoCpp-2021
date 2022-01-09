#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#define START "start"
#define END "end"

class Graph {
  public:
  explicit Graph(const std::set<std::pair<std::string, std::string>> &edges) {
    buildGraph(edges);
  }
  enum class NodeType { Start, End, Small, Large };
  struct Node {
    NodeType nT;
    std::set<std::string> neighbors;
  };

  int findPaths1() {
    std::vector<std::vector<std::string>> paths{};

    for (auto &neighbor: _nodeMap[START].neighbors) {
      std::vector<std::string> path{START, neighbor};
      std::set<std::string> visited{START, neighbor};
      dfs1(neighbor, paths, path, visited);
    }
    return (int) paths.size();
  }
  int findPaths2() {
    std::vector<std::vector<std::string>> paths{};
    for (auto &neighbor: _nodeMap[START].neighbors) {
      std::vector<std::string> path{START, neighbor};
      std::map<std::string, int> visited{{START, 1}, {neighbor, 1}};
      dfs2(neighbor, paths, path, visited);
    }
    return (int) paths.size();
  }

  private:
  std::map<std::string, Node> _nodeMap;

  void buildGraph(const std::set<std::pair<std::string, std::string>> &edges) {
    for (const auto &pair: edges) {
      addNeighbor(pair.first, pair.second);
      addNeighbor(pair.second, pair.first);
    }
  }

  void addNeighbor(const std::string &nodeFrom, const std::string &nodeTo) {
    if (!_nodeMap.contains(nodeFrom)) {
      auto nT = getNodeType(nodeFrom);
      Node node{nT, {nodeTo}};
      _nodeMap.emplace(nodeFrom, node);
    } else {
      _nodeMap[nodeFrom].neighbors.emplace(nodeTo);
    }
  }
  static NodeType getNodeType(const std::string &node) {
    if (node == START) {
      return NodeType::Start;
    } else if (node == END) {
      return NodeType::End;
    } else if (isupper(node[0])) {
      return NodeType::Large;
    }
    return NodeType::Small;
  }
  void dfs1(const std::string &node,
            std::vector<std::vector<std::string>> &paths,
            const std::vector<std::string> &path,
            const std::set<std::string> &visited) {
    if (node == END) {
//      for (const auto &n: path) {
//        std::cout << n << ',';
//      }
//      std::cout << std::endl;
      paths.emplace_back(path);
      return;
    }
    for (auto &n: _nodeMap[node].neighbors) {
      if (hasPossiblePath1(n, visited) && n != START) {
        std::set currentVisited{visited};
        std::vector currentPath{path};
        currentPath.push_back(n);
        currentVisited.emplace(n);
        dfs1(n, paths, currentPath, currentVisited);
      }
    }
  }
  bool hasPossiblePath1(const std::string &node,
                        const std::set<std::string> &visited) {
    if (_nodeMap[node].nT == NodeType::Small && visited.contains(node)) {
      return false;
    }
    return true;
  }
  void dfs2(const std::string &node,
            std::vector<std::vector<std::string>> &paths,
            const std::vector<std::string> &path,
            std::map<std::string, int> &visited) {
    if (node == END) {
//      for (const auto &n: path) {
//        std::cout << n << ',';
//      }
//      std::cout << std::endl;
      paths.emplace_back(path);
      return;
    }
    for (auto &n: _nodeMap[node].neighbors) {
      if (hasPossiblePath2(n, visited) && n != START) {
        std::map currentVisited{visited};
        std::vector<std::string> currentPath{path};
        visit(n, currentVisited);
        currentPath.push_back(n);
        dfs2(n, paths, currentPath, currentVisited);
      }
    }
  }
  bool hasPossiblePath2(const std::string &node,
                        std::map<std::string, int> &visited) {
    for (auto &kV: visited) {
      if (_nodeMap[kV.first].nT == NodeType::Small) {
        if (kV.second == 2) {
          if (_nodeMap[node].nT == NodeType::Small && visited.contains(node) &&
              visited[node] == 1) {
            return false;
          }
        }
      }
    }
    if (_nodeMap[node].nT == NodeType::Small && visited.contains(node) &&
        visited[node]
                                                                 == 2) {
      return false;
    }
    return true;
  }
  static void visit(const std::string &node,
                    std::map<std::string, int> &visited) {
    if (!visited.contains(node)) {
      visited.emplace(node, 1);
    } else {
      visited[node]++;
    }
  }
};

void getInput(const std::string &fileName,
              std::set<std::pair<std::string, std::string>> &edges) {
  std::ifstream ifs{fileName};
  const std::string delim{'-'};
  std::string line{};

  while (std::getline(ifs, line)) {
    size_t next{0};
    size_t last{0};
    while ((next = line.find(delim, last)) != std::string::npos) {
      std::string n1 = line.substr(last, next - last);
      last           = next + 1;
      std::string n2 = line.substr(last);
      edges.emplace(n1, n2);
      //      std::cout << n1 << ',' << n2 << std::endl;
    }
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  std::set<std::pair<std::string, std::string>> edges{};
  getInput(argv[1], edges);
  Graph graph{edges};
  int pathCount1 = graph.findPaths1();
  int pathCount2 = graph.findPaths2();
  std::cout << "Path Count 1: " << pathCount1 << std::endl;
  std::cout << "Path Count 2: " << pathCount2 << std::endl;
  return 0;
}