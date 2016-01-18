/*
* C++ Program to Implement Traveling Salesman Problem using
 Nearest neighbour Algorithm
 */
#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<vector>
#include<ctime>
#include<cstdlib>

using namespace std;

bool testGraph(void);
bool testNode(void);
bool testArc(void);
/*-------------------------------------------------------------
							CLASSES
-------------------------------------------------------------*/
class Arc{
public:
	int start;
	int end;
	int cost;
	int k; // czas
	Arc(){
		start = 0;
		end = 0;
		cost = 0;
		k = 0;
	}
	Arc(int a, int b, int c = 0, int ak = 0){
		start = a;
		end = b;
		cost = c;
		k = ak;
	}
	void print(void){ cout<<"A["<<start<<", "<<end<<"] "; }
	void printAll(void){ cout<<"A["<<start<<", "<<end<<", "<<cost<<"] "; }
};

class Node{
public:
	int i;
	int open;
	int close;
	int service;
	int t;
	vector<int> outputs;
	Node(){
		i = open = close = service = t = 0;
	}
	Node(int a){
 		i = a;
		open = close = service = t = 0; }
	// Node(int a, int b){ Node(a, 0, 0, 0); t = b; }
	Node(int a, int b, int c, int d = 0, int ntime = 0){
		i = a;
		open = b;
		close = c;
		service = d;
		t = ntime;
	}
	Node(int a, int tt){
 		i = a;
 		t = tt;
		open = close = service = 0;
	}
	~Node(){
		while(!outputs.empty()) outputs.pop_back();
	}
	// void setTime(int nt){ t = nt; }
	void addOutput(int a){ outputs.push_back(a); }
	void print(){ cout<<"N("<<i<<") ["<<open<<", "<<close<<", "<<service<<"]"<<endl; }
	void printT(){ cout<<"N("<<i<<") ["<<t<<"]"<<endl; }
};

class Graph{
public:
	vector<Node> nodes;
	vector<Arc> arcs;
	Graph(){};
	~Graph(){
		while(!nodes.empty()) nodes.pop_back();
		while(!arcs.empty()) arcs.pop_back();
	}
	void addNode(Node n){ nodes.push_back(n); }
	void addNode(Node* n){ nodes.push_back(*n); }
	void addNode(int a){ nodes.push_back(Node(a)); }
	void addNode(int a, int b , int c){ nodes.push_back(Node(a, b, c)); }
	void addNode(int a, int b, int c, int d = 0, int e = 0){ nodes.push_back(Node(a, b, c, d)); }
	void addNode(int a, int b){ nodes.push_back(Node(a, b)); }
	void addArc(Arc a){ arcs.push_back(a); }
	void addArc(Arc* a){ arcs.push_back(*a); }
	void addArc(int i, int j, int cost = 0, int k = 0){ arcs.push_back(Arc(i, j, cost, k)); }
	int searchNode(int i){
		int node = 0;
		for(vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it, ++node){
			if(it->i == i) break;
		}
		return node;
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
			if (a != 0){
				while(a < arcs.size()){
					arcs[a].print();
					a = searchNextArc(a);
				}
				cout<<endl;
			}
		}
	}
	void printT(){
		int i, a;
		cout<<"\tNodes("<<nodes.size()<<"):"<<endl;
		for(i=0; i<nodes.size(); ++i) nodes[i].printT();
		cout<<"\tArcs("<<arcs.size()<<"):"<<endl;

		for(i=0; i<nodes.size(); ++i){
			a = searchArc(i);
			if (a != 0){
				while(a < arcs.size()){
					arcs[a].print();
					a = searchNextArc(a);
				}
				cout<<endl;
			}
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

/*-------------------------------------------------------------
							FUNCTIONS
-------------------------------------------------------------*/

Graph* fillGraph(int n, int* tab, int* w){
	// n - wymiar macierzy, gdzie
	// tab [n][n]
	// w   [n][3]
	int i;
	Graph* g = new Graph();
	cout<<"g.nodes size = "<<g->nodes.size()<<endl;
	cout<<"g.arcs size = "<<g->arcs.size()<<endl;
	for(i = 0; i < n; ++i){
		g->addNode(new Node(i));
		for(int j=0; j<n; ++j)
			if(i == j) continue;
			else g->addArc(new Arc(i, j, tab[i*n + j]));
	}
	cout<<"g.nodes size = "<<g->nodes.size()<<endl;
	cout<<"g.arcs size = "<<g->arcs.size()<<endl;

	return g;
}

void printTab(int n, int m, int* t){
	int i, j;
	for(i = 0; i < n; ++i){
		cout<<"[";
		for(j = 0; j < m - 1; ++j) cout<<t[i + j*n]<<", ";
		cout<<t[i + j*n]<<"]\n";
	}
}
void printTab(int n, int* t){ printTab(n,n,t); }

Graph* firstGraph(int n, int* graph, int* windows){
	Graph* g = new Graph();
	int i, j;

	for(i=0; i<n; ++i){
		g->addNode(i, windows[3*i], windows[3*i + 1], windows[3*i + 2]);
		for(j=0; j<n; ++j){
			if(i==j || graph[i*n + j] <= 0) continue;
			g->addArc(i, j, graph[i*n + j]);
		}
	}
	return g;
}

Graph* toSecondGraph(Graph* graph){
	Graph* g = new Graph();
	int n, i, j, p, k, l, cost, cntr;

	// wierzchołki
	n = graph->nodes.size();
	for(i=0; i<n; ++i){
		j = graph->nodes[i].open;
		p = graph->nodes[i].close + 1;
		for(; j<p; ++j){
			g->addNode(i, j);
		}
	}

	// krawędzie
	for(i=0; i<n; ++i) cout<<g->countNodes(i)<<" Nodes "<<i<<endl;

	// dodawanie krawędzi
	for(vector<Node>::iterator vi = g->nodes.begin(); vi != g->nodes.end(); ++vi){
		i = vi->i;
		for(vector<Node>::iterator vj = g->nodes.begin(); vj != g->nodes.end(); ++vj){
			j = vj->i;
			if(i != j){
				aj = graph->nodes.at(graph->searchNode(j)).open
				cost = graph->searchArcCost(i, j); // @TODO brak zmiennych czasów
				if(vj->t == max(aj , vi->t + cost))
				{
					//dodanie krawędzi @TODO brakuje kosztu czekania
					g->addArc(i, j, cost, vi->t);
				}
			}
		}
	}
	// cout<<cntr<<" counter "<<g->nodes.size()<<" g.size()\n";
	// cout<<g->arcs.size()<<" AllArcs\n";

	for(i=0; i<n; ++i){
		p = g->countArcsByStart(i);
		cout<<p<<" Arcs starting at "<<i<<endl;
	}
	// cout<<g->countArcsByStartK(0, 0)<<" Arcs from, 0 time 0\n";

	return g;
}
/*-------------------------------------------------------------
							MAIN
-------------------------------------------------------------*/

int main(){
	srand(time(0));
	//parametry
	// godziny pracy, 0 to pierwsza minuta pracy
	int STARTING_HOUR = 7;
	int STARTING_MIN = 0;
	int ENDING_HOUR = 20;
	int ENDING_MIN = 0;
	int MAX_WORKING_HOURS = 10;
	// GRAF
	int n = 6; // liczba wierzchołków
	int graf[6][6]={{0, 7, 2, 11, 9, 6},
					{7, 0, 15, 5, 8, 10},
					{2, 15, 0, 4, 6, 3},
					{11, 5, 4, 0, 3, 7},
					{9, 8, 6, 3, 0, 7},
					{6, 10, 3, 7, 7, 0}};
	int okna[6][3]={{0, 40, 0},
					{12, 30, 0},
					{4, 9, 0},
					{14, 26, 0},
					{5, 20, 0},
					{1, 13, 0}};
	Graph* g;
	Graph* g2;
	g = firstGraph(n, graf[0], okna[0]);
	// g->print();
	g2 = toSecondGraph(g);
	// g2->printT();

	delete g;
	delete g2;
	return 0;
}
/*-------------------------------------------------------------
						TESTY KLAS
-------------------------------------------------------------*/
bool testArc(void){
	Arc* a = new Arc();
	Arc* b = new Arc(1,5);
	bool result = false;

	if (a->start == 0 && a->end == 0 && b->start == 1 && b->end == 5)
		result = true;
	return result;
}

bool testNode(void){
	Node* a = new Node();
	Node* b = new Node(2);
	bool result = false;

	if (a->i == 0 && b->i == 2)
		result = true;
	return result;
}

bool testGraph(void){
	Graph* g = new Graph();
	Node* newNode = new Node(5);
	Node n(78);
	Arc* newArc = new Arc(5,8);
	Arc a(1,2);
	bool result, result2;

	result = result2 = true;
	g->addNode(n);
	if (g->nodes.size() != 1) result = false;
	g->addNode(*newNode);
	if (g->nodes.size() != 2) result = false;
	g->addNode(newNode);
	if (g->nodes.size() != 3) result = false;

	g->addArc(a);
	if (g->arcs.size() != 1) result2 = false;
	g->addArc(newArc);
	if (g->arcs.size() != 2) result2 = false;
	g->addArc(newArc);
	if (g->arcs.size() != 3) result2 = false;
	return result && result2;
}
