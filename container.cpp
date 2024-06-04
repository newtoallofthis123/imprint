#include "container.h"
using namespace Approach::Render;


Stream *Container::offsetGet(std::string label) {
  auto index = getNodeLabelIndex(label);
  return getLabeledNode(index);
}

void Container::offsetSet(std::string label, Stream *obj) {
  auto offset = getNodeLabelIndex(label);

  this->nodes.push_back(obj);
  if (offset == -1) {
    _labeled_nodes.push_back(this->nodes.size() - 1);
    _node_labels.push_back(label);
  } else {
    _labeled_nodes[offset] = this->nodes.size() - 1;
    _node_labels[offset] = label;
  }
}

Stream *Container::getLabeledNode(int label_index) {
  return this->nodes[_labeled_nodes[label_index]];
}

int Container::getNodeLabelIndex(std::string label) {
  auto offset = std::find(_node_labels.begin(), _node_labels.end(), label);
  if (offset != _node_labels.end()) {
    return offset - _node_labels.begin();
  } else {
    return -1;
  }
}

/*
  Add friend << operator to add a node to the container
*/
inline Approach::Render::Stream &
operator<<(Approach::Render::Container &container,
           Approach::Render::Stream &node) {
  container.nodes.push_back(&node);
  return node;
}

inline Approach::Render::Stream &
operator<<(Approach::Render::Container &container,
           Approach::Render::Stream *node) {
  container.nodes.push_back(node);
  return *node;
}
