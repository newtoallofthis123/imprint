#include "xml.h"

namespace Approach {
namespace Render {
class Imprint {
  Node pattern;
  string output;
  static int count;
  std::map<Node *, string> names;
  Imprint(Node pattern) : pattern(pattern) {}
  Imprint() : pattern(Node()) {}
  ~Imprint() {}

  string get_name(Node *pattern) {
    // TODO: Get the name based on the node
    if (names.find(pattern) != names.end())
      return names[pattern];
    else {
      names[pattern] = "xml" + std::to_string(count);
      count++;
      return names[pattern];
    }
  }

  string export_constructor(Node *pattern) {
    string exported;
    auto xml = static_cast<XML *>(pattern);
    auto name = get_name(pattern);
    exported += "public:\n";
    exported += "std::map<ProcUnit, void *> " + name + "_options;\n";

    if (xml->tag != "") {
      exported += name + "_options[Option::tag] = new std::string(\"" +
                  xml->tag + "\");\n";
    }
    if (xml->id != "") {
      exported += name + "_options[Option::id] = new std::string(\"" + xml->id +
                  "\");\n";
    }
    if (xml->attributes.size() > 0) {
      exported += "std::map<std::string, std::string> attributes;\n";
      for (auto &attr : xml->attributes) {
        exported +=
            "attributes[\"" + attr.first + "\"] = \"" + attr.second + "\";\n";
      }
      exported += name + "_options[Option::attributes] = &attributes;\n";
    }
    if (xml->classes.size() > 0) {
      exported += "std::vector<std::string> classes;\n";
      for (auto &cls : xml->classes) {
        exported += name + "_classes.push_back(\"" + cls + "\");\n";
      }
      exported += name + "_options[Option::classes] = &classes;\n";
    }

    if (xml->content != "") {
      exported += name + "_options[Option::content] = new std::string(\"" +
                  xml->content + "\");\n";
    }

    return exported;
  }

  string export_symbol(Node *pattern) {
    auto name = get_name(pattern);
    return "XML *" + name + " = new XML(" + name + "_options);\n";
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

    return "";
  }

  string export_prefix() {
    return "#include \"xml.h\"\n\n"
           "using namespace Approach::Render;\n\n"
           "class Renderable {\n";
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
