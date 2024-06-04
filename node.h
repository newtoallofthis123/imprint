#include "container.h"

namespace Approach {
namespace Render {
using std::string;
class Node : public Container {
public:
  Node(const string content) {
    this->content = content;
    SetRenderID();
  };
  Node() {
    this->content = "";
    SetRenderID();
  };

  inline friend Node &operator<<(Node &to, Node &node) {
    to.nodes.push_back(&node);
    return to;
  }

  inline friend Node &operator<<(Node &to, Node *node) {
    to.nodes.push_back(node);
    return to;
  }
};
}; // namespace Render
}; // namespace Approach
