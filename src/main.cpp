#include "main.h"

int main (int argc, char **argv) {
  runTests();
  // doGraphTransformation(100);

  cout << "END" << endl;
  return 0;
}


void doGraphTransformation(unsigned int index) {
  unique_ptr<GraphTransformation> gt;
  shared_ptr<Config> conf;
    
  if (index == 100) {
    gt.reset(new GraphTransformation());
  } else {
    conf.reset(new Config(index));
    conf->loadATSP();
    conf->generateTravelTimes();
    conf->generateTimeWindows();
    conf->generateTimeDependency();
    gt.reset(new GraphTransformation(conf));
    gt->firstAuxiliaryGraph();
  }
  gt->print();
  // gt->secondAuxiliaryGraph();
  // gt->print();
}

void runTests() {
  TestNode();
  // TestArc();
  TestGraph();
  unique_ptr<Config> c(new Config());
  c->loadATSP();
  c->generateTravelTimes();
  c->generateTimeWindows();
  c->generateTimeDependency();
  cout << "Tests PASSED" << endl;
}

void TestNode() {
  unique_ptr<Node> n1(new Node(1, 2, 3, 4));
  unique_ptr<Node> n2(new Node(1, 2));
  unique_ptr<Node> n3(new Node(1, 5));
  assert(n1 != NULL);
  assert(n2 != NULL);
  assert(n1->getIndex() == 1);
  assert(n1->getTimeInstance() == 2);
  assert(n1->getOpen() == 3);
  assert(n1->getClose() == 4);
  assert(*n1 == *n2);
  assert(*n1 < *n3);
  assert(*n2 < *n3);
}

void TestArc() {
  cout << "TestArc";
  unique_ptr<Node> n1(new Node(1, 5));
  unique_ptr<Node> n2(new Node(2));
  // unique_ptr<Node> n3(new Node(2));
  // unique_ptr<Node> n4(new Node(2));
  // unique_ptr<Node> n5(new Node(2));

  unique_ptr<Arc> a21(new Arc(*n2, *n1));
  unique_ptr<Arc> a12(new Arc(*n1, *n2));
  
  assert(a21 != NULL);
  assert(a12 != NULL);
  assert(a12->getStart() == 1);
  assert(a12->getTimeInstance() == 5);
  assert(a12->getEnd() == 2);
  assert(a12->getTime() == 0);
  assert(a12->getDistance() == 0);
  n1->print();
  n2->print();
  cout << n2->toString() << "\n";
  a12->print();
  a21->print();
  cout << "123";
  // assert(1 == 2);
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
