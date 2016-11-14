#include <iostream>

class Node {
private:
  unsigned int index;
  unsigned int open;
  unsigned int close;
  unsigned int timeInstance;
public:
  Node(unsigned int index);
  Node(unsigned int index, unsigned int open, unsigned int close, unsigned int timeInstance);
  bool operator<(const Node& n) const;
  unsigned int getIndex() const;
  unsigned int getOpen() const;
  unsigned int getClose() const;
  unsigned int getTimeInstance() const;
  void print() const;
};

Node::Node(unsigned int index) {
  this->index = index;
  open = 0;
  close = 0;
  timeInstance = 0;
}

Node::Node(unsigned int index, unsigned int open, unsigned int close, unsigned int timeInstance) {
  this->index = index;
  this->open = open;
  this->close = close;
  this->timeInstance = timeInstance;
}

bool Node::operator<(const Node& n) const {
  return index < n.index || (index == n.index && timeInstance < n.timeInstance);
}

unsigned int Node::getIndex() const {
  return index;
}

unsigned int Node::getOpen() const {
  return open;
}

unsigned int Node::getClose() const {
  return close;
}

unsigned int Node::getTimeInstance() const {
  return timeInstance;
}

void Node::print() const {
  std::cout << "N(" << index << ", " << timeInstance << ") [" << open << ", " << close << "]" << std::endl;
}
