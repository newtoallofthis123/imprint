#ifndef IMPRINT_H
#define IMPRINT_H

#include "xml.h"
#include <string>
#include <vector>

class Imprint {
private:
  bool inEle, inTag = false;
  std::string buffer;
  std::vector<XML *> children;
  int depth = 0;

public:
  void parse(XML &node, const std::string &xml);
};

#endif
