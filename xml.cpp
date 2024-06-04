#include "xml.h"
using namespace Approach::Render;

void XML::SetOptions(std::map<ProcUnit, void *> options) {
  std::map<ProcUnit, void *>::iterator option;
  for (option = options.begin(); option != options.end(); ++option) {
    switch ((XML::Option)option->first) {
    case Option::tag:
      tag = *(std::string *)option->second;
      break;
    case Option::id:
      id = *(std::string *)option->second;
      break;
    case Option::attributes:
      attributes = *(std::map<std::string, std::string> *)(option->second);
      break;
    default: /* generic option call; */
      break;
    }
  }
}

void XML::prerender(std::ostream &outputstream, const XML &object) {
  this->RenderHead(outputstream);
  this->RenderTail(outputstream);
}

void XML::render(std::ostream &outputstream) {
  this->RenderHead(outputstream);
  this->RenderCorpus(outputstream);
  this->RenderTail(outputstream);
}

void XML::RenderHead(std::ostream &outputstream) {
  // stream opening tag
  outputstream << std::endl << "<"; // open tag
  if (!this->id.empty())
    outputstream << this->tag << " id=\"" << this->id << "\"";
  else
    outputstream << this->tag;

  // stream attributes
  if (!this->attributes.empty()) // if node has attributes
  {
    for (std::map<std::string, std::string>::const_iterator attribute =
             this->attributes.begin();
         attribute != this->attributes.end(); ++attribute) // for each attribute
    {
      outputstream << " " << attribute->first << "=\"" << attribute->second
                   << "\""; // output attribute to stream
    }
  }
  outputstream << ">"; // close tag

  if (!this->content.empty())
    outputstream << std::endl << this->content << std::endl;
}

void XML::RenderCorpus(std::ostream &outputstream) {
  if (!this->nodes.empty())
    for (ProcUnit i = 0, L = this->nodes.size(); i < L; ++i) {
      outputstream << *static_cast<XML *>(this->nodes[i]);
    }
}

void XML::RenderTail(std::ostream &outputstream) {
  outputstream << std::endl << "</" << this->tag << ">";
}
