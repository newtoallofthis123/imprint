#include "xml.h"

namespace Approach {
namespace Render {
class Imprint {
public:
  Node pattern;
  string output;
  int count;
  int depth = 1;
  std::map<Node *, string> names;
  Imprint(Node pattern) : pattern(pattern) {}
  Imprint() : pattern(Node()) {}
  ~Imprint() {}

  string get_name(Node *pattern) {
    // FIXME: Wrong way to determine the name
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

  string export_constructor(Node *pattern) {
    string exported;
    auto xml = static_cast<XML *>(pattern);
    auto name = get_name(pattern);
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
      exported +=
          add_depth() + "std::map<std::string, std::string> attributes;\n";
      for (auto &attr : xml->attributes) {
        exported += add_depth() + "attributes[\"" + attr.first + "\"] = \"" +
                    attr.second + "\";\n";
      }
      exported +=
          add_depth() + name + "_options[Option::attributes] = &attributes;\n";
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

  string export_symbol(Node *pattern) {
    auto name = get_name(pattern);
    return add_depth() + "XML *" + name + " = new XML(" + name + "_options);\n";
  }

  // convert to string such that the
  // children are printed before the parent
  // TODO:
  string print(Node *pattern) {
    string exported;
    if (pattern->nodes.size() == 0) {
      // actual printing
      exported += export_constructor(pattern);
      exported += export_symbol(pattern);
    }

    return exported;
  }

  string export_prefix() {
    return "#include \"xml.h\"\n\n"
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
