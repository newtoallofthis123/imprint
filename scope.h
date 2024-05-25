#include "xml.h"
#include <functional>
#include <memory>
#include <typeindex>

namespace Approach {
namespace Render {
class Scope {
public:
  using Constructor = std::function<std::shared_ptr<Stream>()>;

  static Scope &getInstance() {
    static Scope instance;
    return instance;
  }

  void registerClass(const std::string &name, Constructor constructor) {
    constructors[name] = constructor;
  }

  std::shared_ptr<Stream> createInstance(const std::string &name) {
    return constructors[name]();
  }

  template <typename T> void registerClass(const std::string &name) {
    registerClass(name, []() -> std::shared_ptr<Stream> {
      return std::make_shared<T>();
    });
    classNames[std::type_index(typeid(T))] = name;
  }

  std::string getClassName(std::shared_ptr<Stream> instance) const {
    return classNames.at(std::type_index(typeid(*instance)));
  }

private:
  std::unordered_map<std::string, Constructor> constructors;
  std::unordered_map<std::type_index, std::string> classNames;
};

// Template function to get class from string
template <typename T>
std::shared_ptr<T> getClassFromString(const std::string &className) {
  return std::static_pointer_cast<T>(
      Scope::getInstance().createInstance(className));
}

// Template function to get string from instance
inline std::string
getStringFromInstance(const std::shared_ptr<Stream> &instance) {
  return Scope::getInstance().getClassName(instance);
}

inline void registerClasses() {
  Scope::getInstance().registerClass<XML>("XML");
  Scope::getInstance().registerClass<Node>("Node");
  Scope::getInstance().registerClass<Container>("Container");
}
} // namespace Render
}; // namespace Approach
