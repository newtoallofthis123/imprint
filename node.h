#include "container.h"

namespace Approach {
namespace Render {
using std::string;
class Node : public Container {
public:
  Node(const string content) { this->content = content; };
  Node() { this->content = ""; };
};
}; // namespace Render
}; // namespace Approach
