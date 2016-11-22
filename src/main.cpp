#include <iostream>
#include <memory>
#include <assert.h>
#include "GraphTransformation.cpp"

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;

void TestNode();
void TestArc();
void TestGraph();
void runTests();
void doGraphTransformation(unsigned int index);

int main (int argc, char **argv) {
  runTests();
  // doGraphTransformation(0);

  cout << "END" << endl;
  return 0;
}

void doGraphTransformation(unsigned int index) {
  shared_ptr<Config> conf(new Config(index));
  conf->loadATSP();
  conf->generateTravelTimes();
  conf->generateTimeWindows();
  conf->generateTimeDependency();
  unique_ptr<GraphTransformation> gt(new GraphTransformation());
  gt->load(conf);
  gt->firstAuxiliaryGraph();
  gt->secondAuxiliaryGraph();
}

void runTests() {
  TestNode();
  TestArc();
  TestGraph();
  unique_ptr<Config> c(new Config());
  c->loadATSP();
  c->generateTravelTimes();
  c->generateTimeWindows();
  c->generateTimeDependency();
  cout << "Tests PASSED" << endl;
}

void TestNode() {
  unique_ptr<Node> node(new Node(1, 2, 3, 4));
  unique_ptr<Node> node2(new Node(1, 2));
  unique_ptr<Node> node3(new Node(1, 5));
  assert(node != NULL);
  assert(node2 != NULL);
  assert(node->getIndex() == 1);
  assert(node->getTimeInstance() == 2);
  assert(node->getOpen() == 3);
  assert(node->getClose() == 4);
  assert(*node == *node2);
  assert(*node < *node3);
  assert(*node2 < *node3);
}

void TestArc() {
  unique_ptr<Node> node1(new Node(1));
  unique_ptr<Node> node2(new Node(2));
  unique_ptr<Arc> arc(new Arc(*node2, *node1));
  assert(arc != NULL);
  assert(arc->getDistance() == 0);
  assert(arc->getStart() == 2);
  assert(arc->getEnd() == 1);
}

void TestGraph() {
  const unsigned int NODE_1 = 1;
  const unsigned int NODE_2 = 2;
  const unsigned int NODE_3 = 3;
  unique_ptr<Graph> graph(new Graph());
  unique_ptr<Node> node1(new Node(NODE_1));
  unique_ptr<Node> node2(new Node(NODE_2));
  unique_ptr<Node> node2b(new Node(NODE_2, 5, 0, 0));
  unique_ptr<Node> node3(new Node(NODE_3));
  unique_ptr<Node> node3b(new Node(NODE_3, 0, 1, 2));
  assert(graph != NULL);
  assert(graph->getNodesCount() == 0);
  assert(graph->getArcsCount() == 0);

  graph->addArc(*node1, *node2);
  assert(graph->getArcsCount() == 0);

  graph->addNode(*node1);
  graph->addNode(*node2);
  graph->addNode(*node3);
  graph->addNode(NODE_3);
  graph->addNode(*node3b); // should not add Node(3, 0) already exist
  assert(graph->getNodesCount() == 3);

  graph->addNode(*node3); // should not duplicate
  assert(graph->getNodesCount() == 3);

  graph->addArc(*node1, *node2);
  graph->addArc(*node2, *node3);
  graph->addArc(*node2, *node3); // should not duplicate
  assert(graph->getArcsCount() == 2);

  graph->removeNode(2, 3); // should not remove Node(2, 0) 
  graph->removeNode(3, 0); // should also remove Arcs
  assert(graph->getNodesCount() == 2);
  assert(graph->getArcsCount() == 1);

  // should not remove A(1, 0) => (2, 0)
  graph->removeArc(*node1, *node2b);
  assert(graph->getArcsCount() == 1);

  graph->removeArc(*node1, *node2); // should remove
  assert(graph->getArcsCount() == 0);
}
