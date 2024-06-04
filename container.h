#include "stream.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace Approach {
namespace Render {
class Container : public Stream {
private:
  ProcUnit ActiveRenderCount = 0;

public:
  ProcUnit RenderID;
  std::vector<Stream *> nodes;

  std::vector<std::string> _node_labels;
  std::vector<int> _labeled_nodes;
  std::string content;

  // Set Unique Global Render ID based on static member ActiveRenderCount
  inline const void SetRenderID() {
    RenderID = ActiveRenderCount;
    ++ActiveRenderCount;
  }

  void render(std::ostream &stream) {
    this->RenderHead(stream);
    this->RenderCorpus(stream);
    this->RenderTail(stream);
  }

  void prerender(std::ostream &stream) {
    this->RenderHead(stream);
    this->RenderTail(stream);
  }

  Stream *offsetGet(std::string label);
  void offsetSet(std::string label, Stream *obj);

  Stream *getLabeledNode(int label_index);

  int getNodeLabelIndex(std::string label);

  Stream *operator[](std::string label) { return offsetGet(label); }
  // enable s[0] = new Stream();
  Stream *operator[](int index) { return nodes[index]; }
  // enable setting s[0] = new Stream();
  void operator=(Stream *node) { this->nodes.push_back(node); }

  // add << operator to add a node to the container
  inline Container &operator<<(Stream &node) {
    this->nodes.push_back(&node);
    return *this;
  }
  inline friend Container &operator<<(Stream &node, Container &to) {
    to.nodes.push_back(&node);
    return to;
  }

  // add << operator to add a node to the container
  inline Container &operator<<(Stream *node) {
    this->nodes.push_back(node);
    return *this;
  }

  inline friend Container &operator<<(Stream *node, Container &to) {
    to.nodes.push_back(node);
    return to;
  }

  /** Supports "normal" syntax cout<<XML; is not really a member function */
  inline friend Stream &operator<<(Container &to, Container &node) {
    to.nodes.push_back(&node);
    return to;
  }

  /* Typecast from XML to Node by calling XML.render() into Node.content */
  inline operator std::string() {
    std::ostringstream stream;
    this->prerender(stream);
    return stream.str();
  }

  inline operator const char *() {
    std::ostringstream stream;
    this->prerender(stream);
    auto str = stream.str();
    auto cstr = str.c_str();
    return cstr;
  }

  // Make typecasts friendly

  inline friend Container &operator<<(Container &to, std::string &node) {
    to.content += node;
    return to;
  }

  inline friend Container &operator<<(Container &to, const char *node) {
    to.content += node;
    return to;
  }
};
}; // namespace Render
}; // namespace Approach
