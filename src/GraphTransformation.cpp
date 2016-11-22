#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "Config.cpp"
#include "Route.cpp"
#include "BranchAndBound.cpp"

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
  unsigned int DEPOT_INDEX = config->getVertexCount();
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
  assert(graph->getNodesCount() != 0);

  assert(graph->getArcsCount() == 0);
  loadArcs();
  assert(graph->getArcsCount() != 0);

  connectDepot();
  graph->printNodes();
  graph->printArcs();
}

void GraphTransformation::loadNodes() {
  unsigned int index, timeInstance, open, close;
  for (index = 0; index < config->getVertexCount(); ++index) {
    open = config->getWindows()[index][0];
    close = config->getWindows()[index][1];
    for (timeInstance = open; timeInstance <= close; ++timeInstance)
      graph->addNode(index, timeInstance, open, close);
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
        travel = config->getTimes()[i][j]
                 * config->getTimeDependency()[min(k, max_time)/60] / 1000;
        if (vj->getTimeInstance() == max(
          config->getWindows()[j][0], k + travel)) {
          graph->addArc(*vi, *vj, config->getDistances()[i][j], travel);
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
  
  graph->addNode(DEPOT_INDEX, 0, i, p0);
  for (; i <= p0; ++i) {
    j = 0;
    for (it = arcs.begin(); it != arcs.end(); ++it) {
      // wychodzący
      if (!(j&1) && it->getStart() == 0 && it->getTimeInstance() == i) {
        j = j|1;
        graph->addArc(DEPOT_INDEX, 0, 0, 0);
      }
      // wchodzący
      if (!(j&2) && it->getEnd() == 0 && it->getTimeInstance() == i) {
        j = j|2;
        graph->addArc(0, DEPOT_INDEX, 0, 0);
      }
    }
    if (j == 0) graph->removeNode(0, i); // usunięcie
  }
}

/* reduces near Depot nodes and arcs */
void GraphTransformation::secondAuxiliaryGraph() {
  unsigned int index, j, timeInstance, mint, dist, plus, minus, in, out;
  set<Node>::iterator nit;
  set<Arc>::iterator ait;

  // removing nodes v0s' and v0e'
  for (nit = nodes.begin(); nit != nodes.end();)
    if (nit->getIndex() == 0)
      nit = nodes.erase(nit);
    else
      ++nit;
  
  // moving arcs from v0(0) to vd(DEPOT_INDEX) 
  mint = DEPOT_INDEX;
  for (index = 1; index < config->getVertexCount(); ++index) {
    for (ait = arcs.begin(); ait != arcs.end(); ++ait) {
      if (ait->getStart() == 0 && ait->getEnd() == index) {
        if (ait->getTime() < mint  ||  mint == DEPOT_INDEX) {
          dist = ait->getDistance();
          mint = ait->getTime();
        }
      }
    }
    graph->addArc(DEPOT_INDEX, index, dist, mint);
  }

  for (ait = arcs.begin(); ait != arcs.end(); ++ait)
    if (ait->getEnd() == 0)
      graph->addArc(ait->getStart(), DEPOT_INDEX, ait->getDistance(),
                    ait->getTime());
  
  // removing left arcs
  removeArcsOfNode(0);

  // TODO(pawel) popraw tę pętlę, bo zmieniasz nodes w jej środku (removeNode),
  // więc miałeś segfaulta, gdyż wtedy iterator staje się nieważny. Teraz nie
  // masz segfaulta, ale za to takie przechodzenie przez idx może nie mieć
  // sensu.
  // redukcja zbędnych wierzchołków
  for (nit = nodes.begin(); nit != nodes.end(); ++nit) {
    plus = minus = 0;
    index = nit->getIndex();
    timeInstance = nit->getTimeInstance();
    for (ait = arcs.begin(); ait != arcs.end(); ++ait) {
      if (ait->getStart() == index  &&  ait->getTimeInstance() == timeInstance){
        ++plus;
        out = ait->getEnd();
      }
      if (ait->getEnd() == index
          && ait->getTime()+ait->getTimeInstance() == timeInstance) {
        ++minus;
        in = ait->getStart();
      }
    }
    if (plus == 0 || minus == 0) {
      if (plus + minus > 0)
        removeArcsOfNode(index, timeInstance);
      graph->removeNode(index, timeInstance);
    } else if (plus == 1 && out == DEPOT_INDEX) {
      if (minus == 1 && in == DEPOT_INDEX) {
        removeArcsOfNode(index, timeInstance);
        graph->removeNode(index, timeInstance);
      } else if (minus >= 1) {
        for (j = 0; j < config->getVertexCount(); ++j) {
          if (j == index)
            continue;
          if (config->getWindows()[j][0] >= nit->getOpen() + timeInstance - 1) {
            removeArcsOfNode(index, timeInstance);
            graph->removeNode(index, timeInstance);
          }
        }
      }
    }
  }
  // graph->printNodes();
  // graph->printArcs();
}

void GraphTransformation::removeArcsOfNode(unsigned int index) {
  for (set<Arc>::iterator it = arcs.begin(); it != arcs.end();) {
    if (it->getStart() == index || it->getEnd() == index)
      it = arcs.erase(it);
    else
      ++it;
  }
}

void GraphTransformation::removeArcsOfNode(unsigned int index,
                                           unsigned int timeInstance) {
  for (set<Arc>::iterator it = arcs.begin(); it != arcs.end();) {
    if (it->getTimeInstance() == timeInstance
        && (it->getStart() == index || it->getEnd() == index))
      it = arcs.erase(it);
    else
      ++it;
  }
}