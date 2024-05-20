#include "stream.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace Approach
{
  namespace Render
  {
    // The container class
    class Container : public Stream
    {
    public:
      std::vector<Container *> nodes;
      std::vector<std::string> _node_labels;
      std::vector<int> _labeled_nodes;
      std::string content;

      void render(std::ostream &stream)
      {
        this->RenderHead(stream);
        this->RenderCorpus(stream);
        this->RenderTail(stream);
      }

      void prerender(std::ostream &stream)
      {
        this->RenderHead(stream);
        this->RenderTail(stream);
      }

      Container *offsetGet(std::string label)
      {
        auto index = getNodeLabelIndex(label);
        return getLabeledNode(index);
      }

      void offsetSet(std::string label, Container *obj)
      {
        auto offset = getNodeLabelIndex(label);

        this->nodes.push_back(static_cast<Container *>(obj));

        // if (offset == -1) {
        //   if (_labeled_nodes.empty()) {
        //     _labeled_nodes.push_back(this->nodes.size());
        //   } else {
        //     _labeled_nodes[this->nodes.size()] = this->nodes.size();
        //   }
        //   if (_node_labels.empty()) {
        //     _node_labels.push_back(label);
        //   } else {
        //     _node_labels[this->nodes.size() - 1] = label;
        //   }
        // }
        if (offset == -1)
        {
          _labeled_nodes.push_back(this->nodes.size() - 1);
          if (_node_labels.empty() || this->nodes.size() - 1 == 0)
          {
            _node_labels.push_back(label);
          }
          else
          {
            _node_labels[this->nodes.size() - 1] = label;
          }
        }
      }

      Container *getLabeledNode(int label_index)
      {
        return this->nodes[_labeled_nodes[label_index]];
      }

      int getNodeLabelIndex(std::string label)
      {
        auto offset = std::find(_node_labels.begin(), _node_labels.end(), label);
        if (offset != _node_labels.end())
        {
          return offset - _node_labels.begin();
        }
        else
        {
          return -1;
        }
      }
    };
  };
};