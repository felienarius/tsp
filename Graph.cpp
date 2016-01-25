// Graph.cpp
#include <unistd.h>
#include "Node.cpp"
#include "Arc.cpp"
#include "Data.cpp"

class Graph {
private:
	int n;
	vector<Node> nodes;
	vector<Arc> arcs;
	
	void removeOldDepot() {
		// removing nodes v0s and v0e
		for (vector<Node>::iterator nit = nodes.begin(); nit != nodes.end(); ++nit)
			if (nit->i == 0)
				nodes.erase(nit);
	}
	void moveArcs() {
		// moving arcs from v0(0) to vd(-1)
		int i, dist, t, mink;
		for (i = 1; i < n; ++i) {
			mink = -1;
			for (vector<Arc>::iterator ait = arcs.begin(); ait != arcs.end(); ++ait) {
				if (ait->start == 0 && ait->end == i)
					if (ait->k < mink || mink == -1) {
						dist = ait->dist;
						t = ait->t;
						mink = ait->k;
					}
				if (ait->start == i && ait->end == 0)
					addArc(i, -1, ait->dist, ait->t, ait->k);
			}
			if (mink > -1)
				addArc(-1, i, dist, t, mink);
		}
		// removing left arcs
		removeArcsOfNode(0);
	}
	void reduceNotNeededNodes(Data *d) {
		// redukcja zbędnych wierzchołków
		int plus, minus, in, out, i, k;

		for (vector<Node>::iterator nit = nodes.begin(); nit != nodes.end(); ++nit) {
			plus = minus = 0;
			i = nit->i;
			k = nit->k;
			for (vector<Arc>::iterator ait = arcs.begin(); ait != arcs.end(); ++ait) {
				if (ait->start == i  &&  ait->k == k) {
					++plus;
					out = ait->end;
				}
				if (ait->end == i  &&  ait->t + ait->k == k) {
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
					for (int j = 0; j < n; ++j) {
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
	vector<int> getNodeSeq(int i, int k = 0) {
		vector<int> s;
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if (it->i == i && it->k == k) {
				s = it->outputs;
				break;
			}
		return s;
	}
	void addNode(int i, int k = 0) {
		Node node = Node(i, k);
		nodes.push_back(node);
	}
	void addNode(int i, int open, int close, int k = 0) {
		Node node = Node(i, open, close, k);
		nodes.push_back(node);
	}
	void addArc(int i, int j, int dist, int travel, int k) {
		// vector<Node>::iterator it;
		
		Arc arc = Arc(i, j, dist, travel, k);
		arcs.push_back(arc);
		// for (it = nodes.begin(); it != nodes.end(); ++it) {
		// 	if (it->i == i && it->k == k) {
		// 		it->addOutgoingArc(j);
		// 		break;
		// 	}
		// }
	}
	void removeNode(int i, int k) {
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if (it->i == i && it->k == k) {
				nodes.erase(it);
				break;
			}
	}

	// @TODO remove links to other nodes than removing one
	void removeArcsOfNode(int i, int k) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if ((it->start == i  &&  it->k == k) || (it->end == i  &&  (it->t + it->start == k)))
				arcs.erase(it);
		}
	}
	void removeArcsOfNode(int i) {
		bool any = true;
		while (any) {
			any = false;
			for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
				if (it->start == i || it->end == i) {
					arcs.erase(it);
					any = true;
					break;
				}
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
	// void loadData(Data *d) {
	// 	int i, j;
	// 	vector<Node>::reverse_iterator it;
	// 	n = d->n;
	// 	for (i = 0; i < d->n; ++i) {
	// 		addNode(i, d->windows[i][0], d->windows[i][1]);
	// 		it = nodes.rbegin();
	// 		for (j = 0; j < d->n; ++j) {
	// 			if (i == j) continue;
	// 			addArc(i, j, d->dist[i][j], d->t[i][j]);
	// 			it->addOutgoingArc(j);
	// 		}
	// 	}
	// }

	/* adds Node with all possible time instances */
	void crossNodes(Data *d) {
		int k, i, open, close;

		n = d->n;
		for (i = 0; i < d->n; ++i) {
			open = d->windows[i][0];
			close = d->windows[i][1];
			for (k = open; k <= close; ++k)
				addNode(i, open, close, k);
		}
	}

	/* connects nodes if its traveling time fits (can come earlier!)
	   substracting service time to travelTime  */
	void connectNodes(Data *d) {
		int i, j, k, travel, service;
		vector<Node>::iterator vi, vj;

		for (vi = nodes.begin(); vi != nodes.end(); ++vi) {
			i = vi->i;
			k = vi->k;
			service = d->windows[i][2];
			for (vj = nodes.begin(); vj != nodes.end(); ++vj) {
				j = vj->i;
				if (i != j) {
					travel = (d->t[i][j] + service) * d->f[min(k + service, d->windows[0][1] - 1)/60]/1000;
					if (vj->k == max(d->windows[j][0], k + travel)) {
						addArc(i, j, d->dist[i][j], travel, k);
						vi->addOutgoingArc(j);
					}
				}
			}
		}
	}

	/* Adds Depot = Node(-1) to graph, and link it with other nodes */
	void connectDepot(Data *d) {
		vector<Arc>::iterator it;
		int i, j, p0;

		i = d->windows[0][0];
		p0 = d->windows[0][1];
		addNode(-1, i, p0, 0);
		for (; i <= p0; ++i) {
			j = 0;
			for (it = arcs.begin(); it != arcs.end(); ++it) {
				if (!(j & 1) && (it->start == 0) && (it->k == i)) { // wychodzący
					j = j|1;
					addArc(-1, 0, 0, 0, i);
				}
				if (!(j & 2) && (it->end == 0) && (it->k == i)) { // wchodzący
					j = j|2;
					addArc(0, -1, 0, 0, i);
				}
			}
			if (j == 0) removeNode(0, i); // usunięcie
		}
	}
	
	/* reduces near Depot nodes and arcs */
	void convertToSecondAuxiliaryGraph(Data *d) {
		removeOldDepot();
		moveArcs();
		reduceNotNeededNodes(d);
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
		cout << "Graph(" << n << ")\n";
		cout << "\tNodes(" << nodes.size() << "):" << endl;
		cout << "\tArcs(" << arcs.size() << "):" << endl;
	}
};
