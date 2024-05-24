#include "xml.h"
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
  string get_name(Node *pattern) {
    if (names.find(pattern) != names.end())
      return names[pattern];
    else {
      names[pattern] = "xml" + std::to_string(count);
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

  string exportParamBlock(Node *pattern, string name) {
    string exported;
    auto xml = static_cast<XML *>(pattern);
    exported +=
        add_depth() + "std::map<ProcUnit, void *> " + name + "_options;\n";

    if (xml->tag != "") {
      exported += add_depth() + name +
                  "_options[Option::tag] = new std::string(\"" + xml->tag +
                  "\");\n";
    }
    if (xml->id != "") {
      exported += add_depth() + name +
                  "_options[Option::id] = new std::string(\"" + xml->id +
                  "\");\n";
    }
    if (xml->attributes.size() > 0) {
      exported += add_depth() + "std::map<std::string, std::string> " + name +
                  "_attributes;\n";
      for (auto &attr : xml->attributes) {
        exported += add_depth() + name + "_attributes[\"" + attr.first +
                    "\"] = \"" + attr.second + "\";\n";
      }
      exported += add_depth() + name + "_options[Option::attributes] = &" +
                  name + "_attributes;\n";
    }
    if (xml->classes.size() > 0) {
      exported +=
          add_depth() + "std::vector<std::string>" + name + "_classes;\n";
      for (auto &cls : xml->classes) {
        exported +=
            add_depth() + name + "_classes.push_back(\"" + cls + "\");\n";
      }
      exported += add_depth() + name + "_options[Option::classes] = " + name +
                  "_classes;\n";
    }

    if (xml->content != "") {
      exported += add_depth() + name +
                  "_options[Option::content] = new std::string(\"" +
                  xml->content + "\");\n";
    }
    return exported;
  }

  string exportNodeConstructor(Node *pattern, string name) {
    string exported;
    exported += "XML(" + name + "_options);\n";
    return exported;
  }

  // NOTE: This is a temporary solution to export the symbol
  // The actual implementation needs to me more aware
  string exportNodeSymbol(Node *pattern) {
    auto name = get_name(pattern);
    string exported;
    exported += "XML " + name;
    return exported;
  }

  string exportNode(Node *pattern) {
    auto symbol = exportNodeSymbol(pattern);
    auto constructor = exportNodeConstructor(pattern, get_name(pattern));
    auto params = exportParamBlock(pattern, get_name(pattern));

    string child_exports;
    for (auto &child : pattern->nodes) {
      child_exports += exportNode(static_cast<Node *>(child));
    }

    string exported = params + "\n" + add_depth() + symbol + " = " +
                      constructor + "\n" + child_exports + "\n";
    return exported;
  }

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
};
} // namespace Render
} // namespace Approach
