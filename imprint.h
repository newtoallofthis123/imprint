#include "xml.h"
#include <stack>
#include <string>
#include <vector>

using Approach::Render::Container;
using Approach::Render::Node;
using Approach::Render::Stream;
using Approach::Render::XML;
using std::pair;
using std::stack;
using std::string;

namespace Approach {
namespace Render {
class Imprint : public Approach::Render::Container {
public:
  Container root;
  std::string xml;
  stack<Container *> xmlStack;

  Imprint(const std::string &xml) : xml(xml) {}

  Container *parse() {
    parse_pattern(xml, root);
    return &root;
  }

  /**
    Extract a tag or namespaced tag from a possible starting tag position
    ex: <tag a="a" b="b"> -> tag
    ex: <ns:tag a="a" b="b"> -> ns:tag

    @param xml The string to parse
    @param pos The position to start parsing from
    @return The tag or namespaced tag
  */
  std::string extract_tag(const std::string &xml, int start) {
    int tag_start = xml.find('<', start) + 1;
    int tag_end = xml.find_first_of(" />", tag_start);
    return xml.substr(tag_start, tag_end - tag_start);
  }

  /**
    Parse a string until a control character is found possibly ignoring
    whitespace
    @param xml The string to parse
    @param pos The position to start parsing from
    @param control The control character to look for
    @param ignore_whitespace Whether to ignore whitespace
    @return The position of the control character or the end of the string
  */
  int parse_until(const std::string &xml, int pos, char control,
                  bool ignore_whitespace) {
    while (pos < xml.length()) {
      if (xml[pos] == control) {
        return pos;
      }
      if (xml[pos] == ' ' || xml[pos] == '\t' || xml[pos] == '\n' ||
          xml[pos] == '\r') {
        pos++;
        continue;
      };
      pos++;
    }
    return pos;
  }

  int skip_whitespace(const std::string &xml, int pos) {
    while (pos < xml.length() &&
           (xml[pos] == ' ' || xml[pos] == '\t' || xml[pos] == '\n' ||
            xml[pos] == '\r')) {
      pos++;
    }
    return pos;
  }

  /**
    isValidPatternNamespace - Check if a tag has a valid namespace
    Valid Namespaces: Approach, Imprint, Render, Component

    @param tag The tag to check
    @return bool Whether the tag has a valid namespace
  */
  inline bool isValidPatternNamespace(const std::string &tag) {
    std::vector<std::string> namespaces = {"Approach", "Imprint", "Render",
                                           "Component"};
    for (std::string ns : namespaces) {
      if (tag.find(ns + ":") == 0) {
        return true;
      }
    }
    return false;
  }

  /**
    seek_opening_tag - Seek the opening tag of a node

    @param xml The string to parse
    @param pos The position to start parsing from
    @return std::pair<int, int> The start and end positions of the opening tag
  */
  std::pair<int, int> seek_opening_tag(const std::string &xml, int pos) {
    int start = xml.find('<', pos);
    if (start == std::string::npos) {
      return {-1, -1};
    }
    int end = xml.find('>', start);
    if (end == std::string::npos) {
      return {-1, -1};
    }
    return {start, end + 1};
  }

  std::vector<std::string> split_xml_children(const std::string &input,
                                              int pos = 0) {
    std::vector<std::string> xml_children;

    while (pos < input.size()) {
      pos = skip_whitespace(input, pos);

      // Avoid std::out_of_range
      if (pos >= input.size())
        break;

      if (input[pos] == '<') {
        int start_tag_pos = pos;
        int end_tag_pos = input.find('>', start_tag_pos);
        if (end_tag_pos == std::string::npos)
          break;

        std::string start_tag =
            input.substr(start_tag_pos + 1, end_tag_pos - start_tag_pos - 1);
        int space_pos = start_tag.find(' ');
        std::string tag_name = (space_pos == std::string::npos)
                                   ? start_tag
                                   : start_tag.substr(0, space_pos);

        std::string end_tag = "</" + tag_name + ">";
        int end_tag_start_pos = input.find(end_tag, end_tag_pos);
        if (end_tag_start_pos == std::string::npos)
          break;

        int element_length =
            end_tag_start_pos + end_tag.length() - start_tag_pos;
        xml_children.push_back(input.substr(start_tag_pos, element_length));

        pos = end_tag_start_pos + end_tag.length();
      } else {
        int text_end_pos = input.find('<', pos);
        if (text_end_pos == std::string::npos) {
          xml_children.push_back(input.substr(pos));
          break;
        } else {
          xml_children.push_back(input.substr(pos, text_end_pos - pos));
          pos = text_end_pos;
        }
      }
    }

    return xml_children;
  }

  /**
    parse_pattern - Parse A Pattern File

    1. Seek through string until the start and end of an opening tag are found
      a. Extract the tag name from the opening tag substring
      b. Extract the attributes from the opening tag substring
      c. Create a new Render\XML node wi tag name and attributes
          i) IF the tag has a valid namespace
          ii) ELSE create plain Render\Node
      d. Push the created node to the parent (starts at root Container)
      e. Fill the created node with following contents until next namespaced
    node
    2. Detect the end of the tag
      a. IF the tag is self-closing, continue to the next tag
      b. ELSE recurse until the end of the tag is found

    WARN: the Approach::Render types do not have a parent pointer, so we must
    track it during recursion Note: use the above helper functions to make this
    easier REMINDER: node->parent does not exist. Use current << child; knowing
    that the previous call's current is the parent therefore "parent" is the
    parent of the current node

    @param xml The string to parse
    @param pos The position to start parsing from
    @param parent The parent Stream reference to attach to (all Render\Node,
    Render\Container, Render\XML are Render\Stream)
    @return The parsed tree
  */
  void parse_pattern(const std::string &xml, Container &parent, int pos = 0) {
    Node current;
    while (pos < xml.length()) {
      auto [start, end] = seek_opening_tag(xml, pos);
      if (start == -1) {
        std::cout << xml << std::endl;
        current = Node(xml);
        parent << current;
      }
      pos = end;
      auto tag = extract_tag(xml, start);
      if (!isValidPatternNamespace(tag)) {
        current = Node(xml);
        parent << current;
        continue;
      }
      auto attrs = extract_attrs(xml, start, end);
      auto content = xml.substr(end, xml.find("</" + tag + ">", end) - end);
      std::cout << "TAG: " << tag << std::endl;
      for (auto [key, value] : attrs) {
        std::cout << "ATTR: " << key << " = " << value << std::endl;
      }
      if (isValidPatternNamespace(tag)) {
        XML xml = XML(tag);
        xml.attributes = attrs;
        parent << xml;
      }
      auto spilt_children = split_xml_children(content);
      for (auto e : spilt_children) {
        std::cout << "ELEMENT: " << e << std::endl;
        std::cout << "-v-" << std::endl;
        parse_pattern(e, current, 0);
        parent << current;
      }
      std::cout << std::endl;
    }
  }

  std::map<std::string, std::string> extract_attrs(const std::string &xml,
                                                   int start, int end) {
    std::map<std::string, std::string> attributes;
    int pos = xml.find_first_of(" ", start) + 1;
    while (pos < end) {
      int equals_pos = xml.find('=', pos);
      if (equals_pos == std::string::npos || equals_pos >= end)
        break;
      std::string name = xml.substr(pos, equals_pos - pos);
      int quote_start = xml.find('"', equals_pos) + 1;
      int quote_end = xml.find('"', quote_start);
      std::string value = xml.substr(quote_start, quote_end - quote_start);
      attributes[name] = value;
      pos = quote_end + 1;
    }
    return attributes;
  }
};
}; // namespace Render
}; // namespace Approach
