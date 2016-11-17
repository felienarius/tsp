#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "Config.cpp"

using std::ifstream;
using std::string;
using std::vector;
using std::set;
using std::max;
using std::min;

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;

class GraphTransformation {
private:
  const unsigned int DEPOT_INDEX = 4294967295;
  shared_ptr<Config> config;
  unique_ptr<Graph> graph;
  set<Node> nodes;
  set<Arc> arcs;
  void loadNodes();
  void loadArcs();
  void connectDepot();
  void removeArcsOfNode(int index);
public:
  GraphTransformation();
  void load(shared_ptr<Config> config);
  void firstAuxiliaryGraph();
  void secondAuxiliaryGraph();
};

GraphTransformation::GraphTransformation() {
  graph.reset(new Graph());
}

void GraphTransformation::load(shared_ptr<Config> config) {
  this->config = config;
}

void GraphTransformation::firstAuxiliaryGraph() {
  assert(graph->getNodesCount() == 0);
  loadNodes();
  cout << "Nodes: " << graph->getNodesCount() << endl;
  assert(graph->getNodesCount() != 0);

  assert(graph->getArcsCount() == 0);
  loadArcs();
  cout << "Arcs: " << graph->getArcsCount() << endl;
  assert(graph->getArcsCount() != 0);

  connectDepot();
  cout << "Nodes: " << graph->getNodesCount() << endl;
  cout << "Arcs: " << graph->getArcsCount() << endl;
}

void GraphTransformation::loadNodes() {
  unsigned int k, i, open, close;
  for (i = 0; i < config->getVertexCount(); ++i) {
    open = config->getWindows()[i][0];
    close = config->getWindows()[i][1];
    for (k = open; k <= close; ++k)
      graph->addNode(i, open, close, k);
  }
}

void GraphTransformation::loadArcs() {
  unsigned int i, j, k, travel, max_time = config->getWindows()[0][1] - 1;
  std::set<Node>::iterator vi, vj;

  nodes = graph->getNodes();

  for (vi = nodes.begin(); vi != nodes.end(); ++vi) {
    i = vi->getIndex();
    k = vi->getTimeInstance();
    for (vj = nodes.begin(); vj != nodes.end(); ++vj) {
      j = vj->getIndex();
      if (i != j) {
        travel = config->getTimes()[i][j] * config->getTimeDependency()[min(k, max_time)/60] / 1000;
        if (vj->getTimeInstance() == max(config->getWindows()[j][0], k + travel)) {
          graph->addArc(i, j, config->getDistances()[i][j], travel, k);
        }
      }
    }
  }
}

void GraphTransformation::connectDepot() {
  std::set<Arc>::iterator it;
  unsigned int i, j, p0;

  arcs = graph->getArcs();

  i = config->getWindows()[0][0];
  p0 = config->getWindows()[0][1];
  
  graph->addNode(DEPOT_INDEX, i, p0, 0);
  for (; i <= p0; ++i) {
    j = 0;
    for (it = arcs.begin(); it != arcs.end(); ++it) {
      if (!(j & 1) && (it->getStart() == 0) && (it->getTimeInstance() == i)) { // wychodzący
        j = j|1;
        graph->addArc(DEPOT_INDEX, 0, 0, 0, i);
      }
      if (!(j & 2) && (it->getEnd() == 0) && (it->getTimeInstance() == i)) { // wchodzący
        j = j|2;
        graph->addArc(0, DEPOT_INDEX, 0, 0, i);
      }
    }
    if (j == 0) graph->removeNode(0, i); // usunięcie
  }
}

/* reduces near Depot nodes and arcs */
void GraphTransformation::secondAuxiliaryGraph() {
  int i, j, k, mint, dist, plus, minus, in, out;
  set<Node>::iterator nit;
  set<Arc>::iterator ait;
  // removing nodes v0s' and v0e'
  for (nit = nodes.begin(); nit != nodes.end();)
    if (nit->getIndex() == 0)
      nit = nodes.erase(nit);
    else
      ++nit;
  
  // moving arcs from v0(0) to vd(DEPOT_INDEX) 
  mint = -1;
  k = -1;
  for (i = 1; i < config->getVertexCount(); ++i) {
    for (ait = arcs.begin(); ait != arcs.end(); ++ait) {
      if (ait->getStart() == 0 && ait->getEnd() == i) {
        if (ait->getTime() < mint  ||  mint == -1) {
          dist = ait->getDistance();
          mint = ait->getTime();
          k = ait->getTimeInstance();
        }
      }
    }
    graph->addArc(DEPOT_INDEX, i, dist, mint, k);
  }

  mint = arcs.size();
  for (ait = arcs.begin(); ait != arcs.end(); ++ait)
    if (ait->getEnd() == 0)
      graph->addArc(ait->getStart(), DEPOT_INDEX, ait->getDistance(), ait->getTime(), ait->getTimeInstance());
  
  // removing left arcs
  removeArcsOfNode(0);

  // TODO(pawel) popraw tę pętlę, bo zmieniasz nodes w jej środku (removeNode),
  // więc miałeś segfaulta, gdyż wtedy iterator staje się nieważny. Teraz nie
  // masz segfaulta, ale za to takie przechodzenie przez idx może nie mieć
  // sensu.
  // redukcja zbędnych wierzchołków
  mint = config->getWindows()[0][0];
  for (size_t idx = 0; idx < nodes.size(); ++idx) {
    plus = minus = 0;
    Node* nit = &nodes[idx];
    i = nit->i;
    k = nit->k;
    for (ait = arcs.begin(); ait != arcs.end(); ++ait) {
      if (ait->start == i  &&  ait->k == k){
        ++plus;
        out = ait->end;
      }
      if (ait->end == i  &&  (ait->t + ait->k == k)) {
        ++minus;
        in = ait->start;
      }
    }
    if (plus == 0 || minus == 0){
      if (plus + minus > 0) removeArcsOfNode(i, k);
      removeNode(i, k);
    } else if (plus == 1 && out == DEPOT_INDEX) {
      if (minus == 1 && in == DEPOT_INDEX) {
        removeArcsOfNode(i, k);
        removeNode(i, k);
      } else if (minus >= 1) {
        for (j = 0; j < n; ++j) {
          if (j == i) continue;
          if (d->windows[j][0] >= nit->open + k - 1) {
            removeArcsOfNode(i, k);
            removeNode(i, k);
          }
        }
      }
    }
  }
}

void GraphTransformation::removeArcsOfNode(int index) {
  for (set<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
    if (it->getStart() == index || it->getEnd() == index)
      arcs.erase(it++);
    else
      ++it;
  }
}