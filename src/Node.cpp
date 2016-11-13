class Node {
private:
  unsigned int index;
public:
  Node (unsigned int index);
	bool operator< (const Node& n) const;
  unsigned int getIndex ();
};

Node::Node (unsigned int i) {
  index = i;
}

bool Node::operator< (const Node& n) const {
	return index < n.index;
}

unsigned int Node::getIndex () {
  return index;
}
