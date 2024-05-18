#include "imprint.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

int main() {

  std::ifstream file("test.xml");

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string fileContents = buffer.str();

  std::cout << "__Imprint__" << std::endl;
  std::cout << std::endl;
  std::istringstream is(fileContents);
  Imprint ip(is);
  auto out = ip.parse();

  // std::function<void(const XML &, int)> printXML = [&](const XML &xml,
  //                                                      int depth) {
  //   std::string indent(depth * 2, ' ');
  //   std::cout << indent << "Tag: " << xml.tag << "\n";
  //   std::cout << indent << "Content: " << xml.content << "\n";
  //   for (const auto &child : xml.nodes) {
  //     printXML(*child, depth + 1);
  //   }
  // };
  //
  // printXML(*out, 0);

  std::cout << out->nodes[0]->tag << std::endl;
  std::cout << out->nodes[0]->nodes[0]->tag << std::endl;

  return 0;
}
