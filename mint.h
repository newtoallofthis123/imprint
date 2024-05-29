#include "scope.h"
#include <string>

namespace Approach {
namespace Render {

class Imprint {
public:
  Node pattern;
  string output;
  int count = 1;
  int depth = 1;
  std::map<Node *, string> names;
  Imprint(Node pattern) : pattern(pattern) {}
  Imprint() : pattern(Node()) {}
  ~Imprint() {}

  // NOTE: Wrong way to determine the name
  // To be fixed soon
  string get_name(string typeName, Node *pattern) {
    if (names.find(pattern) != names.end())
      return names[pattern];
    else {
      names[pattern] = typeName + std::to_string(count);
      count++;
      return names[pattern];
    }
  }

  string add_depth() {
    string str = "";
    for (int i = 0; i < depth; i++) {
      str = '\t' + str;
    }
    return str;
  }

  // TODO:
  string exportParameterBlocks(Node *pattern,
                               std::map<ProcUnit, void *> parameters) {
    string exported;

    std::map<ProcUnit, void *>::iterator option;
    for (option = parameters.begin(); option != parameters.end(); ++option) {
      switch (option->first) {}
    }

    return exported;
  }

  template <typename T> string get_class(T *stream) {
    registerClasses();
    auto shared_v = std::make_shared<T>();
    return getStringFromInstance(shared_v);
  }

  string exportNodeConstructor(Node *pattern) {
    string exported;
    auto typeName = get_class(pattern);
    auto name = get_name(typeName, pattern);
    auto statement = typeName + "(" + name + "_options);\n";

    return exported + statement;
  }

  // NOTE: This is a temporary solution to export the symbol
  // The actual implementation needs to me more aware
  string exportNodeSymbol(Node *pattern) {
    auto typeName = get_class(pattern);
    auto id = pattern->RenderID;
    string exported;
    exported += typeName + "_" + std::to_string(id) + ";";
    return exported;
  }

  string exportNode(Node *pattern, Node *parent, string export_symbol) {
    string symbol, constructor, append;
    if (export_symbol == "") {
      symbol = exportNodeSymbol(pattern);
    } else {
      symbol = exportNodeSymbol(pattern);
    }
    constructor = exportNodeConstructor(pattern);

    // TODO: Implement parent and symbol append logic

    string child_exports;
    for (auto &child : pattern->nodes) {
      child_exports += exportNode(static_cast<Node *>(child), nullptr, symbol);
    }

    string exported = "\n" + add_depth() + symbol + " = " + constructor + "\n" +
                      child_exports + "\n";
    return exported;
  }

  string exportNode(Node *pattern, Node *parent) {
    return exportNode(pattern, parent, "");
  }

  string exportNode(Node *pattern) { return exportNode(pattern, nullptr, ""); }

  string print(Node *pattern) {
    string exported;
    exported += exportNode(pattern);

    return exported;
  }

  string export_prefix() {
    return "#include \"xml.h\"\n"
           "#include <map>\n"
           "#include <string>\n"
           "#include <vector>\n"
           "#include <iostream>\n"
           "using namespace Approach::Render;\n\n"
           "int main() {\n";
  }

  string export_end() { return "};"; }

  int Mint(Node *pattern) {
    string file = print(pattern);
    file = export_prefix() + file + export_end();
    output = file;
    return 0;
  }

  void print() { std::cout << output << std::endl; }
};
} // namespace Render
} // namespace Approach
