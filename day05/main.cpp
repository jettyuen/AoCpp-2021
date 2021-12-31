#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

struct Point {
  int x, y;
  bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  bool operator<(const Point &rhs) const {
    return x < rhs.x || (x == rhs.x && y < rhs.y);
  }
};

void getInput(const std::string &fileName, std::map<Point, int> &pointMap);

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Pass puzzle input file as program arg!" << std::endl;
    return -1;
  }
  std::map<Point, int> points{};
  getInput(argv[1], points);
  return 0;
}

void getInput(const std::string &fileName, std::map<Point, int> &pointMap) {
  std::ifstream ifs{fileName};
  std::string line{};
  while (std::getline(ifs, line)) {
    std::istringstream iss{line};
    char c;
    std::string num1{}, num2{}, num3{}, num4{};
    while ((c = iss.get())) {
      if (c == ',') {
        break;
      }
      num1.push_back(c);
    }
    while ((c = iss.get())) {
      if (c == ' ') {
        break;
      }
      num2.push_back(c);
    }
    iss.get();
    iss.get();
    iss.get();
    while ((c = iss.get())) {
      if (c == ',') {
        break;
      }
      num3.push_back(c);
    }
    iss >> num4;
    if (num1 == num3) {
      int x = std::stoi(num1);
      int y1 = std::stoi(num2);
      int y2 = std::stoi(num4);
      if (y2 > y1) {
        while (y2 >= y1) {
          Point point{x, y2};
          if (!pointMap.contains(point)) {
            pointMap.insert(std::pair<Point, int>(point, 0));
          } else {
            pointMap[point] += 1;
            std::cout << "Intersection at: " << point.x << ',' << point.y
                      << std::endl;
          }
          y2--;
        }
      } else {
        while (y2 <= y1) {
          Point point{x, y2};
          if (!pointMap.contains(point)) {
            pointMap.insert(std::pair<Point, int>(point, 0));
          } else {
            pointMap[point] += 1;
            std::cout << "Intersection at: " << point.x << ',' << point.y
                      << std::endl;
          }
          y2++;
        }
      }
    } else if (num2 == num4) {
      int y = std::stoi(num2);
      int x1 = std::stoi(num1);
      int x2 = std::stoi(num3);
      if (x2 > x1) {
        while (x2 >= x1) {
          Point point{x2, y};
          if (!pointMap.contains(point)) {
            pointMap.insert(std::pair<Point, int>(point, 0));
          } else {
            pointMap[point] += 1;
            std::cout << "Intersection at: " << point.x << ',' << point.y
                      << std::endl;
          }
          x2--;
        }
      } else {
        while (x2 <= x1) {
          Point point{x2, y};
          if (!pointMap.contains(point)) {
            pointMap.insert(std::pair<Point, int>(point, 0));
          } else {
            pointMap[point] += 1;
            std::cout << "Intersection at: " << point.x << ',' << point.y
                      << std::endl;
          }
          x2++;
        }
      }
    } else {
      int x1 = std::stoi(num1);
      int x2 = std::stoi(num3);
      int y1 = std::stoi(num2);
      int y2 = std::stoi(num4);
      const int slope = (y2 - y1) / (x2 - x1);
      if (slope == 1) {
        if (x1 > x2) {
          while (x1 >= x2) {
            Point point{x1, y1};
            if (!pointMap.contains(point)) {
              pointMap.insert(std::pair<Point, int>(point, 0));
            } else {
              pointMap[point] += 1;
              std::cout << "Intersection at: " << point.x << ',' << point.y
                        << std::endl;
            }
            y1--;
            x1--;
          }
        } else {
          while (x1 <= x2) {
            Point point{x1, y1};
            if (!pointMap.contains(point)) {
              pointMap.insert(std::pair<Point, int>(point, 0));
            } else {
              pointMap[point] += 1;
              std::cout << "Intersection at: " << point.x << ',' << point.y
                        << std::endl;
            }
            y1++;
            x1++;
          }
        }
      } else if (slope == -1) {
        if (x1 > x2) {
          while (x1 >= x2) {
            Point point{x1, y1};
            if (!pointMap.contains(point)) {
              pointMap.insert(std::pair<Point, int>(point, 0));
            } else {
              pointMap[point] += 1;
              std::cout << "Intersection at: " << point.x << ',' << point.y
                        << std::endl;
            }
            y1++;
            x1--;
          }
        } else {
          while (x1 <= x2) {
            Point point{x1, y1};
            if (!pointMap.contains(point)) {
              pointMap.insert(std::pair<Point, int>(point, 0));
            } else {
              pointMap[point] += 1;
              std::cout << "Intersection at: " << point.x << ',' << point.y
                        << std::endl;
            }
            y1--;
            x1++;
          }
        }
      }
    }
  }
  int ints{};
  ifs.close();
  for (auto &pair: pointMap) {
    if (pair.second > 0) {
      ints++;
    }
  }
  std::cout << "Intersection Count: " << ints << std::endl;
}
