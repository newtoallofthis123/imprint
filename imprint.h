#ifndef IMPRINT_H
#define IMPRINT_H

// #include "xml.h"
#include <stack>
#include <string>
#include <vector>

//! FIXME: With this, it works, but with the actual XML, it doesn't.
class XML {
public:
  std::string tag;
  std::string id;
  std::string content;
  std::vector<XML> nodes;

  XML(std::string tag) : tag(tag){};
};

class Imprint {
private:
  XML root;
  std::stack<XML *> stack;
  std::string given_xml;

  XML parse_xml(const std::string &xml) {

    int pos = 0;
    while (pos < xml.length()) {
      int tag_start = xml.find('<', pos);
      if (tag_start == std::string::npos)
        break;

      int tag_end = xml.find('>', tag_start);
      if (tag_end == std::string::npos)
        break;

      if (xml[tag_start + 1] == '/') {
        stack.pop();
      } else {
        XML temp("");
        int skip_space = xml.find(' ', tag_start);
        int tagname_end;
        if (skip_space != std::string::npos && skip_space < tag_end)
          tagname_end = skip_space;
        else
          tagname_end = tag_end;
        temp.tag = xml.substr(tag_start + 1, tagname_end - tag_start - 1);
        if (skip_space != std::string::npos && skip_space < tag_end) {
          temp.id = xml.substr(skip_space + 1, tag_end - skip_space - 1);
        }
        stack.top()->nodes.push_back(temp);
        stack.push(&stack.top()->nodes.back());
      }

      pos = tag_end + 1;
      int next_tag = xml.find('<', pos);
      if (next_tag != std::string::npos && next_tag > pos) {
        stack.top()->content = xml.substr(pos, next_tag - pos);
        pos = next_tag;
      }
    }

    return root;
  }

public:
  Imprint(std::string xml) : given_xml(xml), root("") { stack.push(&root); };
  Imprint() : root("") { stack.push(&root); };

  XML parse(const std::string &xml) { return parse_xml(xml); };
  XML parse() { return parse_xml(given_xml); }
};

#endif
