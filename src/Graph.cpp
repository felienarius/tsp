#include <set>
#include "Arc.cpp"

using std::set;

class Graph {
 private:
  set<Node> nodes;
  set<Arc> arcs;
 public:
  void addNode(const Node &node);
  void addNode(const unsigned int index, const unsigned int timeInstance = 0,
               const unsigned int open = 0, const unsigned int close = 0);
  void addArc(const Node &start, const Node &end,
              const unsigned int distance = 0, const unsigned int time = 0);
  unsigned int getNodesCount() const;
  unsigned int getArcsCount() const;
  set<Node> getNodes();
  set<Arc> getArcs();
  void removeNode(const unsigned int index,
                  const unsigned int timeInstance = 0);
  void removeArc(const Node &start, const Node &end);
  void printNodes() const;
  void printArcs() const;
};

void Graph::addNode(const Node &node) {
  nodes.insert(node);
}

void Graph::addNode(const unsigned int index, const unsigned int timeInstance,
                    const unsigned int open, const unsigned int close) {
  nodes.insert(Node(index, timeInstance, open, close));
}

void Graph::addArc(const Node &start, const Node &end,
                   const unsigned int distance, const unsigned int time) {
  if (arcs.find(Arc(start, end)) == arcs.end() &&
     nodes.find(start) != nodes.end() &&
     nodes.find(end) != nodes.end()) {
    arcs.insert(Arc(start, end, distance, time));
  }
}

unsigned int Graph::getNodesCount() const {
  return nodes.size();
}

unsigned int Graph::getArcsCount() const {
  return arcs.size();
}

set<Node> Graph::getNodes() {
  return nodes;
}

set<Arc> Graph::getArcs() {
  return arcs;
}

void Graph::removeNode(const unsigned int index,
                       const unsigned int timeInstance) {
  set<Node>::iterator nit = nodes.find(Node(index, timeInstance));
  if (nit != nodes.end()) {
    for (set<Arc>::iterator it = arcs.begin(); it != arcs.end();) {
      if (it->getEnd() == index || it->getStart() == index &&
        it->getTimeInstance() == timeInstance) {
          it = arcs.erase(it);
      } else {
        ++it;
      }
    }
    nodes.erase(nit);
  }
}

void Graph::removeArc(const Node &start, const Node &end) {
  set<Arc>::iterator it = arcs.find(Arc(start, end));
  if (it != arcs.end())
    arcs.erase(it);
}

void Graph::printNodes() const {
  std::cout << "\tNodes(" << nodes.size() << "):" << std::endl;
  for (std::set<Node>::iterator nit = nodes.begin(); nit!= nodes.end(); ++nit) {
    nit->print();
  }
}

void Graph::printArcs() const {
  std::cout << "\tArcs(" << arcs.size() << "):" << std::endl;
  for (std::set<Arc>::iterator aIt = arcs.begin(); aIt != arcs.end(); ++aIt) {
    aIt->print();
  }
}
