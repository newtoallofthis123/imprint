#include "imprint.h"
#include <iostream>

void Imprint::parse(const std::string &xml) {
  char c;
  for (int i = 0; i < xml.length(); i++) {
    c = xml[i];
    if (c == '\n' || c == '\t' || c == ' ')
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
      buffer += c;
    }
  }
}
