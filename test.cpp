#include "xml.h"
#include <iostream>
#include <map>
using namespace Approach::Render;

// template <typename... Args>
// typename std::enable_if<std::is_constructible<std::map<ProcUnit, void *,
// std::less<ProcUnit>, std::allocator<std::pair<const ProcUnit, void *>>>,
// Args...>::value, void>::type create_map(Args &&...Args)
// {
//   return std::map<ProcUnit, void *, std::less<ProcUnit>,
//   std::allocator<std::pair<const ProcUnit, void
//   *>>>(std::forward<Args>(args)...);
// }

// // Implicit conversion of std::map<ProcUnit, void *> from argument list of
// integral type / any time pairs via SFINAE: template <typename... Args>
// typename std::enable_if<std::is_constructible<std::map<ProcUnit, void *>,
// Args...>::value, std::map<ProcUnit, void *>>::type create_map(Args &&...args)
// {
//   return std::map<ProcUnit, void *>(std::forward<Args>(args)...);
// }

std::map<ProcUnit, void *> castToMap(void *ptr) {
  std::pair<ProcUnit, void *> pair1 =
      *static_cast<std::pair<ProcUnit, void *> *>(ptr);
  std::pair<ProcUnit, void *> pair2 =
      *static_cast<std::pair<ProcUnit, void *> *>(
          static_cast<void *>(&pair1) + sizeof(std::pair<ProcUnit, void *>));

  std::map<ProcUnit, void *> result;
  result.emplace(pair1);
  result.emplace(pair2);

  return result;
}

int main() {
  std::map<ProcUnit, void *> options;
  options[(ProcUnit)XML::Option::tag] = new std::string("h1");
  options[(ProcUnit)XML::Option::id] = new std::string("Hello World");

  std::string tag = "h1";
  unsigned long long id = 1234567890;

  XML x({{XML::Option::tag, tag}, {XML::Option::id, id}});

  // Approach::Render::XML xml(opt);

  // std::cout << xml << std::endl;

  // "abc" -> const char *
  // use that as void * since it is const
  // 12 -> int

  std::pair<ProcUnit, void *> pair1{1, new std::string("abc")};
  std::pair<ProcUnit, void *> pair2{1, new int(13)};

  void *ptr = &pair1;

  std::map<ProcUnit, void *> castedMap = castToMap(ptr);

  //////////////////////////////////////////////////
}
