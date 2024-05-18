#include "imprint.h"
#include <sstream>

// Parses a xml string to actual xml using a SAX like parsing method
XML *Imprint::parse() {
  root = new XML("");
  current = root;
  std::string line;
  while (std::getline(input, line)) {
    parse_line(line);
  }
  return root;
}

void Imprint::parse_line(const std::string &line) {
  std::istringstream iss(line);
  std::string token;
  while (iss >> token) {
    if (token[0] == '<') {
      if (token[1] == '/') {
        stack.pop();
        if (!stack.empty()) {
          current = stack.top();
        }
      } else {
        std::string tagName = token.substr(1, token.size() - 1);
        XML *child = new XML(tagName);
        current->nodes.push_back(child);
        stack.push(child);
        current = child;
      }
    } else {
      // if (current)
        current->content += token + " ";
    }
  }
}
