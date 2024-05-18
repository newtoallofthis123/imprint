#include "imprint.h"

void Imprint::parse(XML &node, const std::string &xml) {
  for (auto c : xml) {
    if (c == '\n' || c == '\t' || c == ' ')
      continue;
    if (depth == 0) {
      inTag = false;
      if (!buffer.empty()) {
        auto newNode = XML("");
        parse(newNode, buffer);
      }
    }
    if (c == '<') {
      if (!buffer.empty()) {
        // the actual content
        node.content = buffer;
        buffer.clear();
      }
    } else if (c == '>') {
      if (!buffer.empty()) {
        if (buffer[0] == '/') {
          // end tag
          depth--;
        } else {
          // start tag
          inTag = true;
          depth++;
        }
        if (!inTag)
          buffer.clear();
      }
    } else {
      buffer += c;
    }
  }
}
