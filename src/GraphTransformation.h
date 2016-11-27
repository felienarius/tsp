#ifndef GRAPH_TRANSFORMATION_H
#define GRAPH_TRANSFORMATION_H

class GraphTransformation {
 using std::set;
 using std::max;
 using std::min;
 using std::cout;
 using std::endl;
 using std::unique_ptr;
 using std::shared_ptr;

 private:
  unsigned int DEPOT_INDEX;
  shared_ptr<Config> config;
  unique_ptr<Graph> graph;
  set<Node> nodes;
  set<Arc> arcs;
  void loadNodes();
  void loadArcs();
  void connectDepot();
  void removeArcsOfNode(unsigned int index);
  void removeArcsOfNode(unsigned int index, unsigned int timeInstance);
 public:
  GraphTransformation(shared_ptr<Config> config);
  GraphTransformation();
  void firstAuxiliaryGraph();
  void secondAuxiliaryGraph();
  void runBranchAndBound();
  void print() const;
};
#endif