// graph.cpp
#include "Node.cpp"
#include "Arc.cpp"

class Graph {
private:
	int n;
	vector<Node> nodes;
	vector<Arc> arcs;
	
public:
	Graph(int nn = 0) {
		n = nn;
	};
	~Graph(){
		while(!nodes.empty()) nodes.pop_back();
		while(!arcs.empty()) arcs.pop_back();
	}

	void setNodeI(int i, int k, int newi){
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if(it->i == i && it->t == k)
				it->i = newi;
	}
	
	void addNode(int a){ nodes.push_back(Node(a)); }
	void addNode(int a, int b , int c){ nodes.push_back(Node(a, b, c)); }
	void addNode(int a, int b, int c, int d = 0, int e = 0){ nodes.push_back(Node(a, b, c, d)); }
	void addNode(int a, int b){ nodes.push_back(Node(a, b)); }
	
	void addArc(int i, int j, int cost = 0, int k = 0){ arcs.push_back(Arc(i, j, cost, k)); }
	
	void removeNode(int i, int t){
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if(it->i == i && it->t == t){
				nodes.erase(it);
				return;
			}
	}
	void removeArcsOfNode(int i, int k){
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it){
			if (it->start == i  &&  it->k == k) arcs.erase(it);
			if (it->end == i  &&  (it->cost + it->start == k)) arcs.erase(it);
		}
	}
	int searchNode(int n){
		int index = 0;
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it, ++index){
			if(it->i == n) break;
		}
		return index;
	}
	int searchArc(int a){
		for (int i=0; i<arcs.size(); ++i)
			if (arcs[i].start == a)
				return i;
		return arcs.size();
	}
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
	int searchArcCost(int a, int b){
		for (vector<Arc>::iterator it = arcs.begin(); it != arcs.end(); ++it)
			if (it->start == a)
				if (it->end == b)
					return it->cost;
		return arcs.size();
	}
	int searchNextArc(int a){
		int s = arcs[a].start;
		for(int i=++a; i<arcs.size(); ++i)
				if(arcs[i].start == s)
					return i;
		return arcs.size();	
	}
	void print(){
		int i, a;
		
		cout<<"\tNodes("<<nodes.size()<<"):"<<endl;
		for(i=0; i<nodes.size(); ++i) nodes[i].print();
		
		cout<<"\tArcs("<<arcs.size()<<"):"<<endl;
		for(i=0; i<nodes.size(); ++i){
			a = searchArc(i);
			while(a < arcs.size()){
				arcs[a].print();
				a = searchNextArc(a);
			}
			cout<<endl;
		}
	}
	void printT(){
		int i, a;

		cout<<"\tNodes("<<nodes.size()<<"):"<<endl;
		for (i=0; i<nodes.size(); ++i) nodes[i].printT();
		cout<<"\tArcs("<<arcs.size()<<"):"<<endl;

		for (i = 0; i < nodes.size(); ++i){
			a = searchArc(i);
			while (a < arcs.size()){
				arcs[a].print();
				a = searchNextArc(a);
			}
			cout<<endl;
		}
	}
	int countNodes(int n){
		int sum = 0;
		for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
			if (it->i == n)
				++sum;
		return sum;
	}
};
