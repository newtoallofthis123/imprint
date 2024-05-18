#ifndef IMPRINT_H
#define IMPRINT_H

#include "xml.h"
#include <string>
#include <vector>

class Token {
public:
  const static char OPEN_TAG = '<';
  const static char OPEN_CLOSE = '>';
  const static char CLOSE_END = '>';
  const static char CLOSE_SLASH = '/';
};

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
