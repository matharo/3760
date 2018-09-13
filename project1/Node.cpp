#include <algorithm>
#include <utility>

#include "Node.h"

Node::Node(std::vector<int> state, std::string path)
    : _state(state), _path(path) {}

const std::vector<int> &Node::getState() const { return _state; }

std::string Node::getPath() const { return _path; }
