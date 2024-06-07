#include "scope.h"
#include <string>
#include <utility>

namespace Approach {
namespace Render {

class Imprint {
public:
  Node pattern_node;
  string output;
  static int export_count;
  static int export_depth;
  std::map<string, int> generation_count;
  Imprint(Node pattern) : pattern_node(std::move(pattern)) {}
  Imprint() : pattern_node(Node()) {}
  ~Imprint() = default;

  static string add_depth() {
    string str;
    for (int i = 0; i < export_depth; i++) {
      str += '\t';
    }
    return str;
  }

  // TODO: Actually make this work
  static std::map<string, string>
  exportParameterBlocks(Node *pattern, std::map<ProcUnit, void *> parameters) {
    std::map<string, string> exported;

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

  std::pair<string, string> exportNodeConstructor(Node *pattern) {
    string prepend, assignment;
    auto type = get_class(pattern);

    // FIXME: This is wrong
    std::map<ProcUnit, void *> options;
    auto blocks = exportParameterBlocks(pattern, options);

    string statement = "new " + type + '(';

    for (auto &block : blocks) {
      statement += block.second + ',';
    }

    // TODO: Add the options logic

    return std::make_pair(prepend, statement);
  }

  string exportNodeSymbol(Node *pattern) {
    auto type = get_class(pattern);
    string typeName;

    typeName = type.substr(type.find_last_of(':') + 1);

    if (generation_count.find(typeName) == generation_count.end())
      generation_count[typeName] = 0;

    if (_bound.find(pattern->RenderID) != _bound.end()) {
      _resolved_symbols[pattern->RenderID] = _bound[pattern->RenderID];
    } else {
      generation_count[typeName]++;
    }

    if (_resolved_symbols.find(pattern->RenderID) == _resolved_symbols.end()) {
      _resolved_symbols[pattern->RenderID] =
          typeName + "_" + std::to_string(generation_count[typeName]);
      generation_count[typeName]++;
    }

    return _resolved_symbols[pattern->RenderID];
  }

  string exportNode(Node *pattern, Node parent[], const string &export_symbol) {
    export_count = 0;
    export_depth = 0;
    export_depth++;
    auto tabSpaces = add_depth();

    string symbol;
    if (export_symbol.empty()) {
      symbol = exportNodeSymbol(pattern);
    } else {
      symbol = export_symbol;
    }

    bool append;

    auto constructor = exportNodeConstructor(pattern);

    // TODO: Implement parent and symbol append logic

    string child_exports;
    for (auto &child : pattern->nodes) {
      child_exports += exportNode((Node *)child, nullptr, symbol);
    }

    export_depth--;

    string exported;
    exported += constructor.first + tabSpaces + symbol + " = " +
                constructor.second + ';';
    exported += child_exports;

    return exported;
  }

  // Just to make it easier to call
  string exportNode(Node *pattern, Node *parent) {
    return exportNode(pattern, parent, "");
  }

  string exportNode(Node *pattern) { return exportNode(pattern, nullptr, ""); }

  string print(Node *pattern) {
    string exported;
    exported += exportNode(pattern);

    auto content = "#include \"xml.h\"\n"
                   "#include <map>\n"
                   "#include <string>\n"
                   "#include <vector>\n"
                   "#include <iostream>\n"
                   "using namespace Approach::Render;\n\n"
                   "int main() {\n";
    exported = content + exported + "};";

    return exported;
  }

  int Mint(Node *pattern) {
    string file = print(pattern);
    output = file;
    return 0;
  }

  void print() { std::cout << output << std::endl; }

private:
  std::map<ProcUnit, string> _resolved_symbols;
  std::map<ProcUnit, string> _bound;
};
}; // namespace Render
} // namespace Approach
