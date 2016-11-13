#include <set>
#include "Node.cpp"
#include "Arc.cpp"

using std::set;
using std::cout;
using std::endl;

class Graph {
private:
  set<Node> nodes;
  set<Arc> arcs;
  bool nodeExists(unsigned int index);
  bool arcExists(unsigned int start, unsigned int end);
public:
  void addNode(unsigned int index);
  void addArc(unsigned int start, unsigned int end);
  unsigned int getNodesCount();
  unsigned int getArcsCount();
  void removeNode(unsigned int index);
  void removeArc(unsigned int start, unsigned int end);
};

void Graph::addNode(unsigned int index) {
  nodes.insert(Node(index));
}

void Graph::addArc(unsigned int start, unsigned int end) {
  if (nodeExists(start) && nodeExists(end) && !arcExists(start, end)) {
    arcs.insert(Arc(start, end));
  }
}

unsigned int Graph::getNodesCount() {
  return nodes.size();
}

unsigned int Graph::getArcsCount() {
  return arcs.size();
}

void Graph::removeNode(unsigned int index) {
  for (set<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
    if (it->getStart() == index || it->getEnd() == index)
      arcs.erase(it);
  }
  if (nodeExists(index))
    nodes.erase(nodes.find(Node(index)));
}

void Graph::removeArc(unsigned int start, unsigned int end) {
  arcs.erase(arcs.find(Arc(start, end)));
}

bool Graph::nodeExists(unsigned int index) {
  return nodes.find(Node(index)) != nodes.end();
}

bool Graph::arcExists(unsigned int start, unsigned int end) {
  return arcs.find(Arc(start, end)) != arcs.end();
}
