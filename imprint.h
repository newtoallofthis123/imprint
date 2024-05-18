#ifndef IMPRINT_H
#define IMPRINT_H

#include "xml.h"
#include <stack>
#include <string>

class Imprint {
public:
  Imprint(std::istream &input) : input(input), root(nullptr) {}

  XML* parse();
  // An experimental function for a new parser
  void parse_line(const std::string &line);

private:
  std::istream &input;
  XML *root;
  XML *current;
  std::stack<XML *> stack;
};

#endif
