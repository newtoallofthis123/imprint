#ifndef IMPRINT_H
#define IMPRINT_H

#include <string>

class Imprint {
private:
  bool inEle, inToken = false;
  std::string buffer;

public:
  void parse(const std::string &xml);
};

#endif
