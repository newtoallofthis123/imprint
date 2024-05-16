#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

class Parser {
private:
  bool inEle, inToken = false;
  std::string buffer, token;
  uint8_t wait = 0;

public:
  void handleToken(const std::string &name) {
    std::cout << "Token: " << name << std::endl;
  }

  void parse(const std::string &xml) {
    char c;
    for (int i = 0; i < xml.length(); i++) {
      c = xml[i];
      if (c == '\n' || c == '\t')
        continue;
      if (c == '<') {
        if (!buffer.empty()) {
          std::cout << "c: " << buffer << std::endl;
          buffer.clear();
        }
        inEle = true;
      } else if (c == '>') {
        inEle = false;
        if (!buffer.empty()) {
          if (buffer[0] == '/') {
            auto name = buffer.substr(1);
            std::cout << "e: </" << buffer.substr(1) << ">" << std::endl;
          } else {
            std::cout << "s: <" << buffer << ">" << std::endl;
          }
          buffer.clear();
        }
      } else {
        if (c == '[' && xml.at(i + 1) == '@') {
          inToken = true;
        }
        if (c == '@' && xml.at(i + 1) == ']') {
          handleToken(token);
          token.clear();
          inToken = false;
          wait = 2;
        }
        if (inToken) {
          if (c != '[' && c != '@' && c != '[')
            token += c;
        } else if (wait == 0)
          buffer += c;
        else {
          wait--;
        }
      }
    }
  }
};

int main() {
  Parser p;

  std::ifstream file("test.xml");

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string fileContents = buffer.str();

  p.parse(fileContents);

  return 0;
}
