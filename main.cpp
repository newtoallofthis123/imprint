#include "node.h"
#include <string>
using namespace Approach::Render;

int main() {
  Container c;
  c.content = "Hello, World!";

  c.offsetSet("first", new Node("First Node"));
  c.offsetSet("second", new Node("Second Node"));
  c.offsetSet("second", new Node("Some Node"));
  c.offsetSet("third", new Node("Third Node"));

  auto x= static_cast<Node *>(c["first"]);
  auto y= static_cast<Node *>(c["second"]);
  auto z= static_cast<Node *>(c["third"]);

  std::cout << x->content << std::endl;
  std::cout << y->content << std::endl;
  std::cout << z->content << std::endl;

  return 0;
}
