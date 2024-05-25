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
};
}; // namespace Render
}; // namespace Approach
