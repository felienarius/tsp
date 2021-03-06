#include "Node.h"

Node::Node(const unsigned int index, const unsigned int timeInstance,
           const unsigned int open, const unsigned int close) {
  this->index = index;
  this->timeInstance = timeInstance;
  this->open = open;
  this->close = close;
}

bool Node::operator<(const Node &n) const {
  return (index < n.index || index == n.index && timeInstance < n.timeInstance);
}

bool Node::operator==(const Node &n) const {
  return (index == n.index && timeInstance == n.timeInstance);
}

unsigned int Node::getIndex() const {
  return index;
}

unsigned int Node::getTimeInstance() const {
  return timeInstance;
}

unsigned int Node::getOpen() const {
  return open;
}

unsigned int Node::getClose() const {
  return close;
}

std::string Node::toString() const {
  std::stringstream ss;
  ss << "N(" << index << ", " << timeInstance << ") [" << open << ", "
     << close << "]";
  return ss.str();
}

void Node::print() const {
  std::cout << toString() << std::endl;
}
