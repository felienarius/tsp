#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include "Graph.cpp"

using namespace std;

int RANDOM = 0;

/* nowe*/
void generateDistances(Graph* g, int min, int max, int variation = 10);
void generateTimeWindows(Graph* g, int open, int close, int maxService);
void generateTravelTimes(Graph* g, int min, int max);
Graph* firstAuxiliaryGraph(Graph* g);

/*stare*/
// void printTab(int, int, int*);
// void printTab(int, int*);
// Graph* firstGraph(int, int*, int*);
// Graph* toSecondGraph(Graph*);
// Graph* toSecondGraph(int, int*, int*, int);
// void toThirdGraph(Graph*);


int main() {
	int n = 6; // liczba wierzchołków
	Graph* dg = new Graph(n, 6);
	Graph* tg;

	generateTimeWindows(dg, 0, 40, 0);
	generateDistances(dg, 20, 50);
	generateTravelTimes(dg, 1, 12);
	
	dg->print();
	tg = firstAuxiliaryGraph(dg);
	tg->print();

	delete dg;
	delete tg;

	return 0;
}


/* NOWE */

void generateDistances(Graph* g, int min, int max, int variation /* = 10 */) {
	int i, j, c, n;
	if(RANDOM) srand(time(0));

	n = g->getN();
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if (i == j) continue;
			if (i < j) {
				g->addArc(i, j, rand()%(max - min + 1) + min);
			} else {
				c = g->getArcDist(j, i);
				g->addArc(i, j,
					rand() % (c *(100 + variation)/100 - c *(100 - variation)/100 + 1) + c *(100 - variation)/100);
			}
		}
	}
}

void generateTimeWindows(Graph* g, int open, int close, int maxService) {
	int op, cl, s = 0;

	if(RANDOM) srand(time(0));
	
	g->setTimeWindow(0, open, close, 0);
	for (int i = 1; i < g->getN(); ++i) {
		op = rand() % (close - open + 1) + open;
		cl = rand() % (close - op + 1) + op;
		if(maxService > 0) s = rand() % (maxService + 1);
		g->setTimeWindow(i, op, cl, s);
	}
}

void generateTravelTimes(Graph* g, int min, int max) {
	int i, j, n, t;
	n = g->getN();

	if(RANDOM) srand(time(0));

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if (i == j) continue;
			t = rand() % (max - min + 1) +  min;
			if (i < j) {
				g->setArcTravelTime(i, j, t);
			} else {
				g->setArcTravelTime(i, j, t);
			}
		}
	}
}

Graph* firstAuxiliaryGraph(Graph* graph){
	int n, i, j, p, aj, cost;
	Graph* g;

	n = graph->getN();
	g = new Graph(n);

	// wierzchołki
	for (i = 0; i < n; ++i){
		j = graph->getNodeOpen(i);
		p = graph->getNodeClose(i) + 1;
		for(; j<p; ++j){
			g->addNode(i, j);
		}
	}
	// dodawanie krawędzi
	g->connectNodes(graph);
	

 // 	// dodanie depo(-1)
 // 	g->addNode(-1);
	
	// // dodawanie krawędzi z depo(-1) do 0 i odwrotnie
	// p = graph->nodes.at(graph->searchNode(0)).close -
 // 		graph->nodes.at(graph->searchNode(0)).open + 1;
	// for (i=0; i<p; ++i){
	// 	j=0;
	// 	for (vector<Arc>::iterator it = g->arcs.begin(); it != g->arcs.end(); ++it){
	// 		if(!j&1 && it->start == 0 && it->k == i){ // wychodzący
	// 			j = j|1;
	// 			g->addArc(-1, 0, 0, i);
	// 		}
	// 		if(!j&2 && it->end == 0 && it->k == i){ // wchodzący
	// 			j = j|2;
	// 			g->addArc(0, -1, 0, i);
	// 		}
	// 	}
	// 	if (j == 0) g->removeNode(0, i); // usunięcie 
	// }
	return g;
}
// Graph* firstGraph(int n, int* graph, int* windows){
// 	Graph* g = new Graph(n);
// 	int i, j;

// 	for(i=0; i<n; ++i){
// 		g->addNode(i, windows[3*i], windows[3*i + 1], windows[3*i + 2]);
// 		for(j=0; j<n; ++j){
// 			if(i==j || graph[i*n + j] <= 0) continue;
// 			g->addArc(i, j, graph[i*n + j]);
// 		}
// 	}
// 	return g;
// }

// Graph* fillGraph(int n, int* tab, int* w){
// 	// n - wymiar macierzy, gdzie
// 	// tab [n][n]
// 	// w   [n][3]
// 	int i;
// 	Graph* g = new Graph(n);
// 	cout<<"g.nodes size = "<<g->nodes.size()<<endl;
// 	cout<<"g.arcs size = "<<g->arcs.size()<<endl;
// 	for(i = 0; i < n; ++i){
// 		g->addNode(i);
// 		for(int j=0; j<n; ++j)
// 			if(i == j) continue;
// 			else g->addArc(i, j, tab[i*n + j]);
// 	}
// 	cout<<"g.nodes size = "<<g->nodes.size()<<endl;
// 	cout<<"g.arcs size = "<<g->arcs.size()<<endl;

// 	return g;
// }

void printTab(int n, int m, int* t){
	int i, j;
	for(i = 0; i < n; ++i){
		cout<<"[";
		for(j = 0; j < m - 1; ++j) cout<<t[i + j*n]<<", ";
		cout<<t[i + j*n]<<"]\n";
	}
}
void printTab(int n, int* t){ printTab(n,n,t); }





// Graph* toSecondGraph(int n, int* graph, int* windows, int t){
// 	Graph* g = new Graph(n);
// 	int i, j;

// 	for(i=0; i<n; ++i){
// 		g->addNode(i, windows[3*i], windows[3*i + 1], windows[3*i + 2]);
// 		for(j=0; j<n; ++j){
// 			if(i==j || graph[i*n + j] <= 0) continue;
// 			g->addArc(i, j, graph[i*n + j]);
// 		}
// 	}
// 	return g;
// }

// void toThirdGraph(Graph* g){
// 	int i, k, min, plus, minus, in, out;
// 	vector<Node>::iterator nit;
// 	vector<Arc>::iterator ait;

// 	for (nit = g->nodes.begin(); nit != g->nodes.end(); ++nit)
// 		if (nit->i == 0) g->nodes.erase(nit);
	
// 	// przeniesienie krawędzi zamiast v0
// 	min = -1;
// 	k = 1;
// 	for (i = 1; i < g->n; ++i){
// 		for (ait = g->arcs.begin(); ait != g->arcs.end(); ++ait){
// 			if (ait->start == -1 && ait->end == i){
// 				if (min == -1) min = ait->cost;
// 				if (ait->cost < min){
// 					min = ait->cost;
// 					k = ait->k;
// 				}
// 			}
// 			if (ait->end == 0 && ait->start == i) g->addArc(i, -1, ait->cost, ait->k);
// 		}
// 		g->addArc(-1,i,min,k);
// 	}

// 	// redukcja zbędnych wierzchołków
// 	for (nit = g->nodes.begin(); nit != g->nodes.end(); ++nit){
// 		plus = minus = 0;
// 		i = nit->i;
// 		k = nit->t;
// 		for (ait = g->arcs.begin(); ait != g->arcs.end(); ++ait){
// 			if (ait->start == i  &&  ait->k == k){
// 				++plus;
// 				out = ait->end;
// 			}
// 			if (ait->end == i  &&  (ait->cost + ait->k == k)){
// 				++minus;
// 				in = ait->start;
// 			}
// 		}
// 		if (plus == 0 || minus == 0){
// 			if (plus + minus > 0) g->removeArcsOfNode(i, k);
// 			g->removeNode(i, k);
// 		}
// 		if (plus == 1 && minus == 1){
// 			if(in == -1  && out == -1){
// 				g->removeArcsOfNode(i, k);
// 				g->removeNode(i, k);
// 			}
// 		}
// 	}
// }
