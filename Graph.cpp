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
			if (it->i == i && it->t == k) {
				return it->open;
			}
		}
		return -1;
	}
	int getNodeClose(int i, int k = 0) {
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			if (it->i == i && it->t == k) {
				return it->close;
			}
		}
		return -1;
	}

	void addNode(int i) {
		Node* node = new Node(i);
		nodes.push_back(*node);
	}
	void addArc(int i, int j, int dist, int travel = 0, int k = 0) {
		Arc* arc = new Arc(i, j, dist, travel, k);
		arcs.push_back(*arc);
	}

	void connectNodes(Graph* graph) {
		int i, j, travel;
		for (vector<Node>::iterator vi = nodes.begin(); vi != nodes.end(); ++vi) {
			i = vi->i;
			for (vector<Node>::iterator vj = nodes.begin(); vj != nodes.end(); ++vj) {
				j = vj->i;
				if (i != j) {
					travel = graph->getArcTravelTime(i, j);
					if(vj->t == max(graph->getNodeOpen(j), vi->t + travel)) {
						// @TODO nodes in/outputs missing
						addArc(i, j, graph->getArcDist(i, j), travel, vi->t);
					}
				}
			}
		}
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
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it){
			if (it->start == i  &&  it->end == j){
				it->t = t;
				return;
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
	void addNode(int a, int b){ nodes.push_back(Node(a, b)); }
	
	void setNodeI (int i, int k, int newi){
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if(it->i == i && it->t == k)
				it->i = newi;
	}
	
	
	
	void removeNode(int i, int t){
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if(it->i == i && it->t == t){
				nodes.erase(it);
				return;
			}
	}
	// void removeArcsOfNode(int i, int k){
	// 	for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it){
	// 		if (it->start == i  &&  it->k == k) arcs.erase(it);
	// 		if (it->end == i  &&  (it->cost + it->start == k)) arcs.erase(it);
	// 	}
	// }
	int searchNode(int n){
		int index = 0;
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it, ++index){
			if(it->i == n) break;
		}
		return index;
	}
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
