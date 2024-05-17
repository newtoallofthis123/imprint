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

  std::cout << "__Imprint__" << std::endl;
  std::cout << std::endl;
  ip.parse(fileContents);

  return 0;
}
