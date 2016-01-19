// graph.cpp
#include "Node.cpp"
#include "Arc.cpp"

class Graph {
private:
	int n;
	vector<Node> nodes;
	vector<Arc> arcs;

public:
	~Graph() { clearGraph(); }
	Graph(int vertexCount, int v = 0) {
		n = vertexCount;
		for (int i = 0; i < v; ++i) addNode(i);
	};
	void clearGraph(){
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

	/* get distance between Node i and Node j (in that order) */
	int getArcDist(int i, int j, int k = 0) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it){
			if (it->start == i  &&  it->end == j  &&  it->k ==k)
				return it->dist;
		}
		return 0;
	}
	/* get travel time between Node i and Node j (starting at ) */
	int getArcTravelTime(int i, int j, int k = 0) {
		// @TODO add time dependency
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it){
			if (it->start == i  &&  it->end == j  &&  it->k ==k)
				return it->t;
		}
		return 0;
	}

	void addNode(int i, int k = 0) {
		Node* node = new Node(i, k);
		nodes.push_back(*node);
	}
	void addArc(int i, int j, int dist, int travel = 0, int k = 0) {
		Arc* arc = new Arc(i, j, dist, travel, k);
		arcs.push_back(*arc);
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
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if (it->start == i  &&  it->k == k) arcs.erase(it);
			if (it->end == i  &&  (it->t + it->start == k)) arcs.erase(it);
		}
	}
	void removeArcsOfNode(int i) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if (it->start == i) arcs.erase(it);
			if (it->end == i) arcs.erase(it);
		}
	}

	void setTimeWindow(int i, int open, int close, int service) {
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			if (it->i == i){
				it->open = open;
				it->close = close;
				it->service = service;
				return;
			}
		}
	}

	void setArcTravelTime(int i, int j, int t) {
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it) {
			if (it->start == i  &&  it->end == j){
				it->t = t;
				return;
			}
		}
	}


	/* adds Node with all possible time instances */
	void crossNodes(Graph* graph) {
		int i, j, p;
		for (i = 0; i < n; ++i) {
			j = graph->getNodeOpen(i);
			p = graph->getNodeClose(i) + 1;
			for (; j < p; ++j) {
				addNode(i, j);
			}
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
		addNode(-1);
		setTimeWindow(-1, i, p0, 0);
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
		for (nit = nodes.begin(); nit != nodes.end(); ++nit)
			if (nit->i == 0) {
				nodes.erase(nit);
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
				if (ait->end == 0 && ait->start == i) addArc(i, -1, ait->dist, ait->t, ait->k);
			}
			addArc(-1, i, dist, mint, k);
		}
		// removing left arcs
		removeArcsOfNode(0);

		// redukcja zbędnych wierzchołków
		mint = getNodeOpen(-1);
		for (nit = nodes.begin(); nit != nodes.end(); ++nit) {
			plus = minus = 0;
			i = nit->i;
			k = nit->k;
			for (ait = arcs.begin(); ait != arcs.end(); ++ait) {
				if (ait->start == i  &&  ait->k == k){
					++plus;
					out = ait->end;
				}
				if (ait->end == i  &&  (ait->t + ait->k == k)){
					++minus;
					in = ait->start;
				}
			}
			if (plus == 0 || minus == 0){
				if (plus + minus > 0) removeArcsOfNode(i, k);
				removeNode(i, k);
			} else if (plus == 1 && out == -1) {
				if (minus == 1 && in == -1){
					removeArcsOfNode(i, k);
					removeNode(i, k);
				} else
				if (minus >= 1) {
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
		int i, check;
		cout<<"\tArcs("<<arcs.size()<<"):"<<endl;
		if (!arcs.empty()) {
			cout << "A(i, j)[dist, time, k]\n";
		}
		for (i = 0; i < n; ++i) {
			for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
				if(it->start == i)
					it->print();
			cout<<endl;
		}
	}
	void print(){
		cout<<"Graph(" << n << ")\n";
		printNodes();
		printArcs();
	}












	// void addNode(int a){ nodes.push_back(Node(a)); }
	void addNode(int a, int b , int c){ nodes.push_back(Node(a, b, c)); }
	void addNode(int a, int b, int c, int d = 0, int e = 0){ nodes.push_back(Node(a, b, c, d)); }
	// void addNode(int a, int b){ nodes.push_back(Node(a, b)); }
	
	void setNodeI (int i, int k, int newi){
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if(it->i == i && it->k == k)
				it->i = newi;
	}
	
	
	

	// int searchNode(int n){
	// 	int index = 0;
	// 	for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it, ++index){
	// 		if(it->i == n) break;
	// 	}
	// 	return index;
	// }
	// int searchArc(int a){
	// 	for (int i=0; i<arcs.size(); ++i)
	// 		if (arcs[i].start == a)
	// 			return i;
	// 	return arcs.size();
	// }
	int countArcsByStart(int start){
		int sum = 0;
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
			if(it->start == start)
				++sum;
		return sum;
	}
	int countArcsByStartK(int start, int k){
		int sum = 0;
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
			if(it->start == start)
				if(it->k == k)
					++sum;
		return sum;
	}
	// int searchArcCost(int a, int b){
	// 	for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
	// 		if (it->start == a)
	// 			if (it->end == b)
	// 				return it->cost;
	// 	return arcs.size();
	// }
	int searchNextArc(int a){
		int s = arcs[a].start;
		for(int i=++a; i<arcs.size(); ++i)
				if(arcs[i].start == s)
					return i;
		return arcs.size();	
	}

	// void printT(){
	// 	int i, a;

	// 	cout<<"\tNodes("<<nodes.size()<<"):"<<endl;
	// 	for (i=0; i<nodes.size(); ++i) nodes[i].printT();
	// 	cout<<"\tArcs("<<arcs.size()<<"):"<<endl;

	// 	for (i = 0; i < nodes.size(); ++i){
	// 		a = searchArc(i);
	// 		while (a < arcs.size()){
	// 			arcs[a].print();
	// 			a = searchNextArc(a);
	// 		}
	// 		cout<<endl;
	// 	}
	// }
	int countNodes(int n){
		int sum = 0;
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if (it->i == n)
				++sum;
		return sum;
	}
};
