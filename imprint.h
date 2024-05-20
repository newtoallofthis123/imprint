#ifndef IMPRINT_H
#define IMPRINT_H

#include "xml.h"
#include <stack>
#include <string>
#include <vector>

namespace Approach
{
  namespace Render
  {
    class Imprint
    {
    private:
      XML root;
      std::string given_xml;

      XML parse_xml(const std::string &xml);

    public:
      Imprint(std::string xml) : given_xml(xml), root(""){};
      Imprint() : root(""){};

      XML parse(const std::string &xml) { return parse_xml(xml); };
      XML parse() { return parse_xml(given_xml); }
    };
  };
};

#endif
