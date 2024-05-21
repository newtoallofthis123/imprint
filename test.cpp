#include "xml.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;
using namespace Approach::Render;

int main() {
  // use options to call the xml class
  std::map<ProcUnit, void *> options;
  options[Option::tag] = new std::string("h1");
  options[Option::id] = new std::string("main-title");

  auto x = XML(options);

  cout << x << endl;

  return 0;
}
