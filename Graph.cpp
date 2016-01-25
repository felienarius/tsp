// Graph.cpp
#include "Node.cpp"
#include "Arc.cpp"
#include "Data.cpp"

class Graph {
private:
	int n;
	vector<Node> nodes;
	vector<Arc> arcs;

public:
	Graph(int vertexCount = 0, int v = 0) {
		n = vertexCount;
		for (int i = 0; i < v; ++i) addNode(i);
	}
	~Graph() {
		while (!arcs.empty()) arcs.pop_back();
		while (!nodes.empty()) nodes.pop_back();
	}
	int getN() { return n; }
	int getNodeOpen(int i, int k = 0) {
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			if (it->i == i && it->k == k) {
				return it->open;
			}
		}
		return -1;
	}
	int getNodeClose(int i, int k = 0) {
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			if (it->i == i && it->k == k) {
				return it->close;
			}
		}
		return -1;
	}
	int getNodesCount() { return nodes.size(); }
	int getArcsCount() { return arcs.size(); }

	/* get distance between Node i and Node j (in that order) */
	int getArcDist(int i, int j, int k = 0) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if (it->start == i  &&  it->end == j  &&  it->k ==k)
				return it->dist;
		}
		return 0;
	}
	/* get travel time between Node i and Node j (starting at ) */
	int getArcTravelTime(int i, int j, int k = 0) {
		// @TODO add time dependency
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if (it->start == i  &&  it->end == j  &&  it->k ==k)
				return it->t;
		}
		return 0;
	}
	void addNode(int i, int k = 0) {
		nodes.push_back(Node(i, k));
	}
	void addNode(int i, int open, int close, int service, int k = 0) {
		nodes.push_back(Node(i, open, close, service,k));
	}
	void addArc(int i, int j, int dist, int travel = 0, int k = 0) {
		Arc* arc = new Arc(i, j, dist, travel, k);
		arcs.push_back(*arc);
		// for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
		// 	if (it->i == i && it->k == k)
		// 		it->addOutgoingArc(j);
	}

	void removeNode(int i, int k) {
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if (it->i == i && it->k == k) {
				removeArcsOfNode(i, k);
				nodes.erase(it);
				break;
			}
	}

	// @TODO remove links to other nodes than removing one
	void removeArcsOfNode(int i, int k) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end();) {
      bool removed = false;
			if (it->start == i  &&  it->k == k) {
        it = arcs.erase(it);
        removed = true;
      }
			if (it->end == i  &&  (it->t + it->start == k)) { 
        it = arcs.erase(it);
        removed = true;
      }
      if (!removed) {
        ++it;
      }
		}
	}
	void removeArcsOfNode(int i) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if (it->start == i) arcs.erase(it);
			if (it->end == i) arcs.erase(it);
		}
	}
	void setArcTravelTime(int i, int j, int t) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if (it->start == i  &&  it->end == j) {
				it->t = t;
				break;
			}
		}
	}
	void loadData(Data *d) {
		int i, j;
		vector<Node>::reverse_iterator it;
		n = d->n;
		for (i = 0; i < d->n; ++i) {
			addNode(i, d->windows[i][0], d->windows[i][1], d->windows[i][2]);
			it = nodes.rbegin();
			for (j = 0; j < d->n; ++j) {
				if (i == j) continue;
				addArc(i, j, d->dist[i][j], d->t[i][j]);
				it->addOutgoingArc(j);
			}
		}
	}

	/* adds Node with all possible time instances */
	void crossNodes(Graph* graph) {
		vector<Node>::iterator it;
		int k, i, open, close, service;

		n = graph->n;
		for (it = graph->nodes.begin(); it != graph->nodes.end(); ++it) {
			i = it->i;
			open = it->open;
			close = it->close;
			service = it->service;
			for (k = open; k <= close; ++k)
				addNode(i, open, close, service, k);
		}
	}

	/* connects nodes if its traveling time fits (can come earlier!) */
	void connectNodes(Graph* graph) {
		int i, j, travel;
		for (vector<Node>::iterator vi = nodes.begin(); vi != nodes.end(); ++vi) {
			i = vi->i;
			for (vector<Node>::iterator vj = nodes.begin(); vj != nodes.end(); ++vj) {
				j = vj->i;
				if (i != j) {
					travel = graph->getArcTravelTime(i, j);
					if(vj->k == max(graph->getNodeOpen(j), vi->k + travel)) {
						// @TODO nodes in/outputs missing
						addArc(i, j, graph->getArcDist(i, j), travel, vi->k);
					}
				}
			}
		}
	}

	/* Adds Depot = Node(-1) to graph, and link it with other nodes */
	void connectDepot(Graph* graph) {
		int i, j, p0;
		i = graph->getNodeOpen(0);
		p0 = graph->getNodeClose(0); // - i + 1;
		// k = {0, ... , p0}
		addNode(-1, i, p0, 0);
		p0 = p0 - i + 1;
		for (i = 0; i < p0 ; ++i) {
			j = 0;
			for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
				if(!(j & 1) && (it->start == 0) && (it->k == i)) { // wychodzący
					j = j|1;
					addArc(-1, 0, 0, 0, i);
				}
				if(!(j & 2) && (it->end == 0) && (it->k == i)) { // wchodzący
					j = j|2;
					addArc(0, -1, 0, 0, i);
				}
			}
			if (j == 0) removeNode(0, i); // usunięcie
		}
	}
	
	/* reduces near Depot nodes and arcs */
	void convertToSecondAuxiliaryGraph(Graph* graph) {
		int i, j, k, mint, dist, plus, minus, in, out;
		vector<Node>::iterator nit;
		vector<Arc>::iterator ait;

		// removing nodes v0s' and v0e'
		for (nit = nodes.begin(); nit != nodes.end();)
			if (nit->i == 0) {
				nit = nodes.erase(nit);
			} else {
        ++nit;
      }
		
		// moving arcs from v0(0) to vd(-1) 
		mint = -1;
		k = -1;
		for (i = 1; i < n; ++i) {
			for (ait = arcs.begin(); ait != arcs.end(); ++ait) {
				if (ait->start == 0 && ait->end == i) {
					if (ait->t < mint  ||  mint == -1) {
						dist = ait->dist;
						mint = ait->t;
						k = ait->k;
					}
				}
				if (ait->end == 0 && ait->start == i)
					addArc(i, -1, ait->dist, ait->t, ait->k);
			}
			addArc(-1, i, dist, mint, k);
		}
		// removing left arcs
		removeArcsOfNode(0);

		// redukcja zbędnych wierzchołków
		mint = getNodeOpen(-1);
    // TODO(pawel) popraw tę pętlę, bo zmieniasz nodes w jej środku (removeNode),
    // więc miałeś segfaulta, gdyż wtedy iterator staje się nieważny. Teraz nie
    // masz segfaulta, ale za to takie przechodzenie przez idx może nie mieć
    // sensu.
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
			} else if (plus == 1 && out == -1) {
				if (minus == 1 && in == -1) {
					removeArcsOfNode(i, k);
					removeNode(i, k);
				} else if (minus >= 1) {
					for (j = 0; j < n; ++j) {
						if (j == i) continue;
						if (graph->getNodeOpen(j) >= nit->open + k - 1) {
							removeArcsOfNode(i, k);
							removeNode(i, k);
						}
					}
				}
			}
		}
	}


	void printNodes() {
		cout<<"\tNodes("<<nodes.size()<<"):"<<endl;
		if (!nodes.empty()) {
			cout <<"N(#, t) [open, close, service]\n";
		}
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			it->print();
	}
	void printArcs() {
		vector<Arc>::iterator it;
		bool any;
		int i;

		cout<<"\tArcs("<<arcs.size()<<"):"<<endl;
		if (!arcs.empty()) {
			cout << "A(i, j)[dist, time, k]\n";
			for (i = -1; i < n; ++i) {
				any = false;
				for (it = arcs.begin(); it != arcs.end(); ++it)
					if(it->start == i) {
						it->print();
						cout << endl;
						any = true;
					}
				if (any) cout << endl;
			}
		}
	}
	void print() {
		cout<<"Graph(" << n << ")\n";
		printNodes();
		printArcs();
	}
	void printShort() {
		cout<<"Graph(" << n << ")\n";
		cout<<"\tNodes("<<nodes.size()<<"):"<<endl;
		cout<<"\tArcs("<<arcs.size()<<"):"<<endl;
	}
};
