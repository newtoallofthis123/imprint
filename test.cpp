#include "scope.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
using namespace std;
using namespace Approach::Render;

template <typename T> string get_class(T *stream) {
  registerClasses();
  auto x_ptr = std::make_shared<T>();
  return getStringFromInstance(x_ptr);
}

int main() {
  registerClasses();
  std::map<ProcUnit, void *> options;
  options[Option::tag] = new std::string("h1");
  options[Option::id] = new std::string("main-title");

  auto x = XML(options);

  // auto x_ptr = std::make_shared<XML>(x);
  // cout << getStringFromInstance(x_ptr) << endl;

  cout << get_class(&x) << endl;

  return 0;
}
