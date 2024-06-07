#include "container.h"
#include <map>

namespace Approach {
namespace Render {
using std::string;
class Node : public Container {
public:
  enum class Option : ProcUnit {
    content,
  };
  explicit Node(const string &content) {
    this->content = content;
    SetRenderID();
  };
  Node() {
    this->content = "";
    SetRenderID();
  };

  explicit Node(const std::map<ProcUnit, void *> &options) {
    Node::SetRenderID();
    SetOptions(options);
  }

  void SetOptions(std::map<ProcUnit, void *> options) {
    std::map<ProcUnit, void *>::iterator option;
    for (option = options.begin(); option != options.end(); ++option) {
      switch ((Node::Option)option->first) {
      case Option::content:
        content = *(std::string *)option->second;
        break;
      default: /* generic option call; */
        break;
      }
    }
  }

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
