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
    if (c == Token::OPEN_TAG) {
      if (!buffer.empty()) {
        // the actual content
        node.content = buffer;
        buffer.clear();
      }
    } else if (c == Token::OPEN_CLOSE) {
      if (!buffer.empty()) {
        if (buffer[0] == Token::CLOSE_SLASH) {
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
