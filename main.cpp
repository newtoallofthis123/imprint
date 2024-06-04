#include "imprint.h"
#include <fstream>
#include <string>
using namespace Approach::Render;

/* int main() {
  // read test.xml to a string
  std::ifstream file("test.xml");

  std::string xml;
  std::string line;
  while (std::getline(file, line)) {
    xml += line;
  }

  // create an Imprint object
  Imprint i(xml);

  auto x = i.parse();

  std::cout << x->nodes.size() << std::endl;

  auto y = (XML *)x->nodes[0];

  std::cout << *y; 

  return 0;
} */

int main(){
  XML x("h1");

  std::cout << x;
}
