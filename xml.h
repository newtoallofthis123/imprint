#include "node.h"
#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace Approach {
namespace Render {
class XML : public Node {

public:
  enum class Option : ProcUnit {
    id = 0,
    tag,
    attributes,
    content,
  };

  std::string tag, id;
  std::map<std::string, std::string> attributes;
  std::vector<std::string> classes;

  /************************
   *   CONSTRUCTORS        *
   *                       *
   * Supply NULL to skip an argument
   * XML(tag [,options])
   * XML(tag,id [,options])
   * XML(tag,id,classes [,options])
   * XML(tag,id,classes,attributes [,options])
   * XML(tag,id,classes,attributes [,options])
   *                       *
   *                       */

  /* Strict Typing */

  XML(const std::string &_tag)
  throw() : tag(_tag) { XML::SetRenderID(); }
  XML(const std::string &_tag, const std::string &_id)
  throw() : tag(_tag), id(_id) { XML::SetRenderID(); }
  XML(const std::string &_tag, const std::string &_id,
      std::vector<std::string> &_classes)
  throw() : tag(_tag), id(_id), classes(_classes) { XML::SetRenderID(); }
  XML(const std::string &_tag, const std::string &_id,
      std::vector<std::string> &_classes,
      std::map<std::string, std::string> &_attributes)
  throw() : tag(_tag), id(_id), classes(_classes), attributes(_attributes) {
    XML::SetRenderID();
  }

  XML(std::string &_tag)
  throw() : tag(_tag) { XML::SetRenderID(); }
  XML(std::string &_tag, std::string &_id)
  throw() : tag(_tag), id(_id) { XML::SetRenderID(); }
  XML(std::string &_tag, std::string &_id, std::vector<std::string> &_classes)
  throw() : tag(_tag), id(_id), classes(_classes) { XML::SetRenderID(); }
  XML(std::string &_tag, std::string &_id, std::vector<std::string> &_classes,
      std::map<std::string, std::string> &_attributes)
  throw() : tag(_tag), id(_id), classes(_classes), attributes(_attributes) {
    XML::SetRenderID();
  }

  /* Mixed Typing */

  XML() { XML::SetRenderID(); }

  XML(const std::string &_tag, std::map<ProcUnit, void *> options)
  throw() : tag(_tag) {
    XML::SetRenderID();
    SetOptions(options);
  }
  XML(const std::string &_tag, const std::string &_id,
      std::map<ProcUnit, void *> options)
  throw() : tag(_tag), id(_id) {
    XML::SetRenderID();
    SetOptions(options);
  }
  XML(const std::string &_tag, const std::string &_id,
      std::vector<std::string> &_classes, std::map<ProcUnit, void *> options)
  throw() : tag(_tag), id(_id), classes(_classes) {
    XML::SetRenderID();
    SetOptions(options);
  }
  XML(const std::string &_tag, const std::string &_id,
      std::vector<std::string> &_classes,
      std::map<std::string, std::string> &_attributes,
      std::map<ProcUnit, void *> options)
  throw() : tag(_tag), id(_id), classes(_classes), attributes(_attributes) {
    XML::SetRenderID();
    SetOptions(options);
  }

  /* Options only */

  XML(std::map<ProcUnit, void *> options) {
    XML::SetRenderID();
    SetOptions(options);
  }

  /************************
   *       ACTIONS         *
   *                       */

  void SetOptions(std::map<ProcUnit, void *> options);

  /************************
   *   STREAM TO CLASS     *
   *                       */

  /** Nests child nodes into the instance by pointer */
  inline void operator<<(Node *object) {
    this->nodes.push_back(static_cast<XML *>(object));
  }

  /** Nests child nodes into the instance by reference */
  inline void operator<<(Node &object) {
    this->nodes.push_back(static_cast<XML *>(&object));
  }

  /************************
   *   RENDERING PIPELINE  *
   *                       */

  void prerender(std::ostream &outputstream, const XML &object);
  void render(std::ostream &outputstream);

  /** Outputs this node's tag, id and attributes to the stream. */
  void RenderHead(std::ostream &outputstream);

  /** Outputs any child nodes to stream. */
  void RenderCorpus(std::ostream &outputstream);

  /** Outputs closing tag to stream. */
  void RenderTail(std::ostream &outputstream);

  /************************
   *   STREAM OPERATORS    *
   *                       */

  /** Funky XML>>cout syntax, works in situations without the non-member
   * friend
   */
  inline void operator>>(std::ostream &outputstream) {
    this->render(outputstream);
  }

  /** Supports "normal" syntax cout<<XML; is not really a member function */
  inline friend std::ostream &operator<<(std::ostream &outputstream, XML &obj) {
    obj.render(outputstream);
    return outputstream;
  }
};
}; // namespace Render
}; // namespace Approach
