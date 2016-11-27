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
  void getTestGraph();
  void printNodes() const;
  void printArcs() const;
  void print() const;
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

void Graph::getTestGraph() {
  unsigned int start, end;
  set<Node>::iterator n1, n2;

  start = 1, end = 7;
  for (unsigned int i = start; i <= end; ++i)
    addNode(0, i, start, end);
  start = 2, end = 4;
  for (unsigned int i = start; i <= end; ++i)
    addNode(1, i, start, end);
  start = 2, end = 3;
  for (unsigned int i = start; i <= end; ++i)
    addNode(2, i, start, end);
  start = 4, end = 6;
  for (unsigned int i = start; i <= end; ++i)
    addNode(3, i, start, end);
  //V0
  n1 = nodes.find(Node(0, 1));
  n2 = nodes.find(Node(1, 3));
  addArc(*n1, *n2, 2, 82);
  n2 = nodes.find(Node(2, 2));
  addArc(*n1, *n2, 1, 42);
  n2 = nodes.find(Node(3, 4));
  addArc(*n1, *n2, 3, 115);

  n1 = nodes.find(Node(0, 2));
  n2 = nodes.find(Node(1, 3));
  addArc(*n1, *n2, 1, 42);
  n2 = nodes.find(Node(2, 3));
  addArc(*n1, *n2, 1, 39);
  n2 = nodes.find(Node(3, 4));
  addArc(*n1, *n2, 2, 78);

  n1 = nodes.find(Node(0, 3));
  n2 = nodes.find(Node(1, 4));
  addArc(*n1, *n2, 1, 42);
  n2 = nodes.find(Node(3, 5));
  addArc(*n1, *n2, 2, 77);
  
  n1 = nodes.find(Node(0, 4));
  addArc(*n1, *n2, 1, 40);

  n1 = nodes.find(Node(0, 5));
  n2 = nodes.find(Node(3, 6));
  addArc(*n1, *n2, 1, 41);
  
  //V1
  n1 = nodes.find(Node(1, 2));
  n2 = nodes.find(Node(0, 3));
  addArc(*n1, *n2, 1, 41);
  n2 = nodes.find(Node(2, 3));
  addArc(*n1, *n2, 1, 40);
  n2 = nodes.find(Node(3, 4));
  addArc(*n1, *n2, 2, 79);

  n1 = nodes.find(Node(1, 3));
  n2 = nodes.find(Node(0, 4));
  addArc(*n1, *n2, 1, 40);
  n2 = nodes.find(Node(3, 4));
  addArc(*n1, *n2, 1, 40);

  n1 = nodes.find(Node(1, 4));
  n2 = nodes.find(Node(0, 5));
  addArc(*n1, *n2, 1, 42);
  n2 = nodes.find(Node(3, 5));
  addArc(*n1, *n2, 1, 39);

  //V2
  n1 = nodes.find(Node(2, 2));
  n2 = nodes.find(Node(0, 3));
  addArc(*n1, *n2, 1, 41);
  n2 = nodes.find(Node(1, 3));
  addArc(*n1, *n2, 1, 41);
  n2 = nodes.find(Node(3, 4));
  addArc(*n1, *n2, 2, 83);

  n1 = nodes.find(Node(2, 3));
  n2 = nodes.find(Node(0, 4));
  addArc(*n1, *n2, 1, 40);
  n2 = nodes.find(Node(1, 4));
  addArc(*n1, *n2, 1, 38);
  n2 = nodes.find(Node(3, 4));
  addArc(*n1, *n2, 1, 42);

  //V3
  n1 = nodes.find(Node(3, 4));
  n2 = nodes.find(Node(0, 5));
  addArc(*n1, *n2, 1, 42);
  
  n1 = nodes.find(Node(3, 5));
  n2 = nodes.find(Node(0, 6));
  addArc(*n1, *n2, 1, 41);
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

void Graph::print() const {
  std::cout << "\tGraph:\n";
  printNodes();
  printArcs();
}
