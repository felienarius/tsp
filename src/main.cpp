#include <iostream>
#include <memory>
#include <assert.h>
#include "GraphTransformation.cpp"


// using namespace std;
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
  doGraphTransformation(0);
  
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
}

void runTests() {
  TestArc();
  TestNode();
  TestGraph();
  unique_ptr<Config> c(new Config());
  c->loadATSP();
  c->generateTravelTimes();
  c->generateTimeWindows();
  c->generateTimeDependency();
  cout << "Tests PASSED" << endl;
}

void TestNode() {
  unique_ptr<Node> node(new Node(0));
  assert(node != NULL);
  assert(node->getIndex() == 0);
}

void TestArc() {
  unique_ptr<Arc> arc(new Arc(2, 1));
  assert(arc != NULL);
  assert(arc->getValue() == 0);
  assert(arc->getStart() == 2);
  assert(arc->getEnd() == 1);
  arc->setValue(15);
  assert(arc->getValue() == 15);
}

void TestGraph() {
  unique_ptr<Graph> graph(new Graph());
  assert(graph != NULL);
  assert(graph->getNodesCount() == 0);
  assert(graph->getArcsCount() == 0);
  graph->addArc(5, 18);
  assert(graph->getArcsCount() == 0);
  graph->addNode(8);
  graph->addNode(11);
  graph->addNode(58);
  assert(graph->getNodesCount() == 3);
  graph->addNode(11);
  assert(graph->getNodesCount() == 3);
  graph->addArc(8, 11);
  graph->addArc(11, 58);
  graph->addArc(11, 58);
  assert(graph->getArcsCount() == 2);
  graph->removeNode(8);
  graph->removeNode(8);
  assert(graph->getNodesCount() == 2);
  assert(graph->getArcsCount() == 1);
}
