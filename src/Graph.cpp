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
  bool arcExists(unsigned int start, unsigned int end);
public:
  void addNode(unsigned int index, unsigned int open, unsigned int close, unsigned int timeInstance);
  void addArc(unsigned int start, unsigned int end);
  void addArc(unsigned int start, unsigned int end, unsigned int distance, unsigned int time, unsigned int  timeInstance);
  unsigned int getNodesCount();
  unsigned int getArcsCount();
  set<Node> getNodes();
  set<Arc> getArcs();
  void removeNode(unsigned int index, unsigned int timeInstance);
  void removeArc(unsigned int start, unsigned int end);
  void printNodes();
};

void Graph::addNode(unsigned int index, unsigned int open, unsigned int close, unsigned int timeInstance) {
  nodes.insert(Node(index, open, close, timeInstance));
}

void Graph::addArc(unsigned int start, unsigned int end) {
  arcs.insert(Arc(start, end));
}

void Graph::addArc(unsigned int start, unsigned int end, unsigned int distance, unsigned int time, unsigned int  timeInstance) {
  arcs.insert(Arc(start, end, distance, time, timeInstance));
}

unsigned int Graph::getNodesCount() {
  return nodes.size();
}

unsigned int Graph::getArcsCount() {
  return arcs.size();
}

set<Node> Graph::getNodes() {
  return nodes;
}

set<Arc> Graph::getArcs() {
  return arcs;
}

void Graph::removeNode(unsigned int index, unsigned int timeInstance) {
  for (set<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
    if (it->getStart() == index || it->getEnd() == index)
      if (it->getTimeInstance() == timeInstance)
        arcs.erase(it);
  }
  set<Node>::iterator nit = nodes.find(Node(index));
  if (nit != nodes.end())
    nodes.erase(nit);
}

void Graph::removeArc(unsigned int start, unsigned int end) {
  arcs.erase(arcs.find(Arc(start, end, 0, 0, 0)));
}

// bool Graph::nodeExists(unsigned int index) {
//   return nodes.find(Node(index)) != nodes.end();
// }

bool Graph::arcExists(unsigned int start, unsigned int end) {
  return arcs.find(Arc(start, end, 0, 0, 0)) != arcs.end();
}

void Graph::printNodes() {
  cout << "Printing Nodes(" << nodes.size() << "):" << endl;
  for(std::set<Node>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
    i->print();
  }
}
