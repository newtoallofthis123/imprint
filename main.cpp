#include "mint.h"
#include <string>
using namespace Approach::Render;

int main() {
  XML x("h1");
  x.id = "header";
  x.content = "Hello, World!";

  XML y("p");
  y.id = "paragraph";

  XML z("a");
  z.id = "link";

  y << z;
  x << y;

  Imprint m(x);
  m.Mint(&x);

  std::cout << m.output << std::endl;

  return 0;
}
