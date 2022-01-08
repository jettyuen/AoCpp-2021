#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#define BAD_ANGLE_BRACE 25137
#define BAD_CURLY_BRACE 1197
#define BAD_BRACKET 57
#define BAD_PAREN 3

#define AUTO_ANGLE_BRACE 4
#define AUTO_CURLY_BRACE 3
#define AUTO_BRACKET 2
#define AUTO_PAREN 1

void getInput(const std::string &fileName, std::vector<std::string> &lines) {
  std::ifstream ifs{fileName};
  std::string line{};
  while (std::getline(ifs, line)) {
    lines.push_back(line);
  }
}
int findSyntaxErrors(const std::vector<std::string> &lines) {
  int badParens{}, badBrackets{}, badCurlyBraces{}, badAngleBraces{};
  for (const auto &line: lines) {
    std::stack<char> opens{};
    for (const auto &c: line) {
      if (c == '{' || c == '[' || c == '(' || c == '<') {
        opens.push(c);
      } else if ((c == '}' && opens.top() == '{') ||
                 (c == ']' && opens.top() == '[') ||
                 (c == ')' && opens.top() == '(') ||
                 (c == '>' && opens.top() == '<')) {
        opens.pop();
      } else {
        std::cout << line << " - Expected to close " << opens.top()
                  << " but "
                     "got "
                  << c << " instead." << std::endl;
        if (c == '>') {
          badAngleBraces += BAD_ANGLE_BRACE;
        } else if (c == '}') {
          badCurlyBraces += BAD_CURLY_BRACE;
        } else if (c == ']') {
          badBrackets += BAD_BRACKET;
        } else if (c == ')') {
          badParens += BAD_PAREN;
        }
        break;
      }
    }
  }
  return badAngleBraces + badCurlyBraces + badBrackets + badParens;
}
long scoreAutoComplete(const std::vector<std::string> &lines) {
  std::vector<long> scores{};
  for (const auto &line: lines) {
    std::stack<char> opens{};
    auto i{0};
    for (const auto &c: line) {
      if (c == '{' || c == '[' || c == '(' || c == '<') {
        opens.push(c);
      } else if ((c == '}' && opens.top() == '{') ||
                 (c == ']' && opens.top() == '[') ||
                 (c == ')' && opens.top() == '(') ||
                 (c == '>' && opens.top() == '<')) {
        opens.pop();
      } else {
        break;
      }
      if (i == line.size() - 1) {
        std::queue<int> completions{};
        while (!opens.empty()) {
          char cc = opens.top();
          if (cc == '<') {
            completions.push(AUTO_ANGLE_BRACE);
          } else if (cc == '{') {
            completions.push(AUTO_CURLY_BRACE);
          } else if (cc == '[') {
            completions.push(AUTO_BRACKET);
          } else if (cc == '(') {
            completions.push(AUTO_PAREN);
          }
          opens.pop();
        }
        long score{0};
        while (!completions.empty()) {
          score *= 5;
          score += completions.front();
          completions.pop();
        }
        scores.push_back(score);
        break;
      }
      i++;
    }
  }
  std::sort(scores.begin(), scores.end());
  return scores[scores.size() / 2];
}
int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Pass puzzle input as program argument!" << std::endl;
    return 0;
  }
  std::vector<std::string> lines{};
  getInput(argv[1], lines);
  int syntaxErrorScore   = findSyntaxErrors(lines);
  long autoCompleteScore = scoreAutoComplete(lines);
  std::cout << "Syntax Error Score: " << syntaxErrorScore << std::endl;
  std::cout << "Auto-Complete Score: " << autoCompleteScore << std::endl;
  return 0;
}
