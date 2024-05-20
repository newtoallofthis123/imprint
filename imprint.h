#ifndef IMPRINT_H
#define IMPRINT_H

#include "xml.h"
#include <string>
#include <vector>
#include <stack>

using Approach::Render::Container;
using Approach::Render::Node;
using Approach::Render::Stream;
using Approach::Render::XML;
using std::pair;
using std::stack;
using std::string;

namespace Approach
{
  namespace Render
  {


    class Imprint : public Approach::Render::Container
    {
    public:
      Container root;
      std::string given_xml;
      static stack<Container*> stack;

      /**
        Recursively parse nodes and their contents

        @param node The node to parse
        @param xml The string to parse
        @param pos The position to start parsing from
      */
      void parse_nodes(Node *node, const std::string &xml, int &pos)
      {
        while (pos < xml.length())
        {
          if (xml[pos] == '<')
          {
            int start = pos;
            pos++;
            if (xml[pos] == '/')
            {
              pos = parse_until(xml, pos, '>', true);
              pos++;
              return;
            }
            std::string tag = extract_tag(xml, pos);
            pos += tag.length();
            pos = parse_until(xml, pos, '>', true);
            pos++;
            // Node *child = new Node(tag);
            // node->nodes.push_back(child);
            // child->parent = node;
            // parse_nodes(child, xml, pos);
          }
          else
          {
            int start = pos;
            pos = parse_until(xml, pos, '<', false);
            node->content = xml.substr(start, pos - start);
          }
        }
      }

      /**
        Extract a tag or namespaced tag from a possible starting tag position
        ex: <tag a="a" b="b"> -> tag
        ex: <ns:tag a="a" b="b"> -> ns:tag

        @param xml The string to parse
        @param pos The position to start parsing from
        @return The tag or namespaced tag
      */
      std::string extract_tag(const std::string &xml, int pos)
      {
        int start = pos;
        while (pos < xml.length())
        {
          if (xml[pos] == ' ' || xml[pos] == '>')
          {
            return xml.substr(start, pos - start);
          }
          pos++;
        }
        return "";
      }

      /**
        Parse a string until a control character is found possibly ignoring whitespace
        @param xml The string to parse
        @param pos The position to start parsing from
        @param control The control character to look for
        @param ignore_whitespace Whether to ignore whitespace
        @return The position of the control character or the end of the string
      */
      int parse_until(const std::string &xml, int pos, char control, bool ignore_whitespace)
      {
        while (pos < xml.length())
        {
          if (xml[pos] == control)
          {
            return pos;
          }
          if (ignore_whitespace && xml[pos] == ' ' || xml[pos] == '\t' || xml[pos] == '\n' || xml[pos] == '\r')
          {
            pos++;
            continue;
          };
        }
        return pos;
      }

      /**
        isValidPatternNamespace - Check if a tag has a valid namespace
        Valid Namespaces: Approach, Imprint, Render, Component

        @param tag The tag to check
        @return bool Whether the tag has a valid namespace
      */
      inline bool isValidPatternNamespace(const std::string &tag)
      {
        std::vector<std::string> namespaces = {"Approach", "Imprint", "Render", "Component"};
        for (std::string ns : namespaces)
        {
          if (tag.find(ns + ":") == 0)
          {
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

      std::pair<int, int> seek_opening_tag(const std::string &xml, int pos)
      {
        int start = pos;
        while (pos < xml.length())
        {
          if (xml[pos] == '<')
          {
            start = pos;
            pos++;

            std::string tag = extract_tag(xml, pos);

            // Check if the tag has a valid namespace
            if (!isValidPatternNamespace(tag))
            {
              // Ignore the tag and continue
              pos = parse_until(xml, pos, '>', true);
              pos++;
              continue;
            }
            //<Imprint:  cursor is here, before the tag name itself, after the :

            // Check our current position for the end of the tag
            pos += tag.length();

            // Then we keep moving
            pos = parse_until(xml, pos, '>', true);
            pos++;

            // Return the start and end positions of the opening tag
            return std::make_pair(start, pos);
          }
          pos++;
        }
        return std::make_pair(start, pos);
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
          e. Fill the created node with following contents until next namespaced node
        2. Detect the end of the tag
          a. IF the tag is self-closing, continue to the next tag
          b. ELSE recurse until the end of the tag is found

        Note: the Approach::Render types do not have a parent pointer, so we must track it during recursion
        Note: use the above helper functions to make this easier
        REMINDER: node->parent does not exist. Use current << child; knowing that the previous call's current is the parent
        therefore "parent" is the parent of the current node

        @param xml The string to parse
        @param pos The position to start parsing from
        @param parent The parent Stream reference to attach to (all Render\Node, Render\Container, Render\XML are Render\Stream)
        @return The parsed tree
      */
      void parse_pattern(const std::string &xml, int pos = 0, Container &parent)
      {
        Node current;

        while (pos < xml.length())
        {
          std::pair<int, int> tag = seek_opening_tag(xml, pos);

          // Copy between the last position and the start of the tag into current.content
          current.content += xml.substr(pos, tag.first - pos);
          parent << current;

          // Create a new node with the tag name and attributes
          std::string tag_name = extract_tag(xml, tag.first + 1);
          auto attrs = extract_attrs(xml, tag.first, tag.second);
          XML * pattern_node = new XML(tag_name);
          parent << pattern_node;

          // Recurse into the new node
          stack.push(pattern_node);
          parse_pattern(xml, tag.second, &pattern_node);
          stack.pop(pattern_node);

          // Move the position to the end of the tag
          pos = tag.second;
        }
      }

    public:
      Stream stack;

      /**
        Imprint - Constructor

        @param xml The XML string to parse
      */
      Imprint(const std::string &xml) : given_xml(xml) {}

      /**
        parse - Parse the XML string

        @return The root container
      */
      Container *parse()
      {
        parse_pattern(given_xml, 0, root);
        return &root;
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
          e. Fill the created node with following contents until next namespaced node
        2. Detect the end of the tag
          a. IF the tag is self-closing, continue to the next tag
          b. ELSE recurse until the end of the tag is found

        Note: the Approach::Render types do not have a parent pointer, so we must track it during recursion
        Note: use the above helper functions to make this easier
        REMINDER: node->parent does not exist. Use current << child; knowing that the previous call's current is the parent
        therefore "parent" is the parent of the current node

        @param xml The string to parse
        @param pos The position to start parsing from
        @param parent The parent Stream reference to attach to (all Render\Node, Render\Container, Render\XML are Render\Stream)
        @return The parsed tree
      * /
      void parse_pattern(const std::string &xml, int pos = 0, XML &parent)
      {
        Node current("");

        while (pos < xml.length())
        {
          std::pair<int, int> tag = seek_opening_tag(xml,

          stack.push(node);
          parse_pattern(xml, pos, node);
          stack.pop(node);

          // std::string tag_name = extract_tag(xml, tag.first + 1);
          // std::string tag_attributes = xml.substr(tag.first, tag.second - tag.first);
          // XML node(tag_name, tag_attributes);
          // parent << node;
          // pos = tag.second;
        }
      }
      */

      /**
        extract_attrs - Extract the attributes from an opening tag

        @param xml The string to parse
        @param start The start position of the opening tag
        @param end The end position of the opening tag
        @return std::vector<std::pair<std::string, std::string>> The attributes of the tag
      */
      std::vector<std::pair<std::string, std::string>> extract_attrs(const std::string &xml, int start, int end)
      {
        std::vector<std::pair<std::string, std::string>> attrs;
        int pos = start;
        while (pos < end)
        {
          if (xml[pos] == ' ')
          {
            pos++;
            continue;
          }
          int attr_start = pos;
          pos = parse_until(xml, pos, '=', true);
          std::string attr_name = xml.substr(attr_start, pos - attr_start);
          pos++;
          pos = parse_until(xml, pos, '"', true);
          pos++;
          int attr_val_start = pos;
          pos = parse_until(xml, pos, '"', false);
          std::string attr_val = xml.substr(attr_val_start, pos - attr_val_start);
          attrs.push_back(std::make_pair(attr_name, attr_val));
          pos++;
        }
        return attrs;
      }
    };
  };
};

#endif
