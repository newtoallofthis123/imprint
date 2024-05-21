#include "mint.h"
#include <string>
using namespace Approach::Render;

int main() {
  XML x("h1");
  x.id = "header";
  x.classes.push_back("header");

  Imprint m(x);
  m.Mint(&x);

  std::cout << m.output << std::endl;

  return 0;
}
