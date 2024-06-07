#include "imprint.h"
#include <fstream>
#include <string>
using namespace Approach::Render;

int main() {
  // read test.xml to a string
  std::ifstream file("test.xml");

  std::string xml;
  std::string line;
  while (std::getline(file, line)) {
    xml += line;
  }

  // create an Imprint object
  Imprint i(xml);

  XML cn("Component:Node");
  XML n("node");
  XML n1("node");
  n.content = "<div>hello</div>";
  n << &n1;
  cn << &n;

  std::cout << cn << std::endl;

  i.parse();

  // auto x = i.Mint(&cn);

  return 0;
}
