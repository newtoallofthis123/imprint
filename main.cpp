#include "imprint.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  Imprint ip;

  std::ifstream file("test.xml");
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string fileContents = buffer.str();

  auto root = ip.parse(fileContents).nodes[0];

  // For debugging
  // std::cout << root.nodes[0].tag;
  std::cout << "Root Node: " << root.tag << std::endl;
  std::cout << "Attributes: " << root.id << std::endl;
  std::cout << "Content: " << root.content << std::endl;
  for (auto child : root.nodes) {
    std::cout << "Child Node: " << child.tag << std::endl;
    std::cout << "Attributes: " << child.id << std::endl;
    std::cout << "Content: " << child.content << std::endl;
  }

  return 0;
}
