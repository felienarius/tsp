#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "Graph.cpp"

using namespace std;

int RANDOM = 0;


void loadDistances(Graph* g, int n, int** dist);
void loadTimeWindows(Graph* g, int n, int** windows);
void loadTravelTimes(Graph* g, int n, int** times);
Graph* firstAuxiliaryGraph(Graph* g);

int loadATSP(string name, int*** dist);
int** calcTimeTravel(int n, int** dist, int max_t);
int** generateTimeWindows(int n, int close, int maxService, int seed = 5);
int* generateHourMultipler(int n, int variation = 15);
void printTab(int n, int m, int** t);


int main() {
	int m;
	int workHours = 12;
	int** dist;
	int** windows;
	int** t;
	int* f;
	m = loadATSP(string("br17"), &dist);
	t = calcTimeTravel(m, dist, workHours * 60);
	windows = generateTimeWindows(m, workHours * 60, 10);
	f = generateHourMultipler(workHours);
	// printTab(m, 3, windows);
	// printTab(1, 12, &f);
	
	for (int i = 0; i < m; ++i) {
		delete [] dist[i];
		delete [] windows[i];
		delete [] t[i];
	}
	delete []dist;
	delete []windows;
	delete []t;

	// int n = 6; // liczba wierzchołków
	// Graph* dg = new Graph(n, 6);
	// Graph* tg;

	// loadDistances(dg, n, dist);
	// generateTimeWindows(dg, 0, 41, 0);
	// generateTravelTimes(dg, 1, 12);
	// tg = firstAuxiliaryGraph(dg);
	// tg->convertToSecondAuxiliaryGraph(dg);

	// delete dg;
	// delete tg;

	return 0;
}


/* NOWE */

void loadDistances(Graph* g, int n, int** dist) {
	int i, j;

	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j) {
			if (i == j) continue;
			g->addArc(i, j, dist[i][j]);
		}
}

void loadTimeWindows(Graph* g, int n, int** windows) {
	for (int i = 0; i < n; ++i) {
		g->setTimeWindow(i, windows[i][0], windows[i][1], windows[i][2]);
	}
}

void loadTravelTimes(Graph* g, int n, int** times) {
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			if (i == j) continue;
			g->setArcTravelTime(i, j, times[i][j]);
		}
}

Graph* firstAuxiliaryGraph(Graph* graph){
	int n, i, j, p, aj, cost;
	Graph* g;

	n = graph->getN();
	g = new Graph(n);

	// vertexes
	g->crossNodes(graph);

	// adding arcs
	g->connectNodes(graph);

	// adding Depot(-1)
	g->connectDepot(graph);
	
	return g;
}

int loadATSP(string name, int*** dist) {
	ifstream plik;
	string line;
	int i, j, n;
	int** d;

	plik.open((string("atsp/") + name + string(".atsp")).c_str());
	if (!plik.good()) return 0;

	while (line.find("DIMENSION:")) { plik >> line; }
	plik >> n;
	d = new int*[n];
	for (i = 0; i < n; ++i) {
		d[i] = new int[n];
	}
	while (line.find("EDGE_WEIGHT_SECTION")) { // getline (plik, line) ) {//pętla nieskończona
		plik >> line;
	}
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			plik >> d[i][j];
	plik.close();
	*dist = d;
	return n;
}

int** calcTimeTravel(int n, int** dist, int max_t) {
	int i, j, sum, s;
	int** d;

	d = new int*[n];
	for (i = 0; i < n; ++i) {
		d[i] = new int[n];
	}

	sum = 0;
	for (i = 0; i < n; ++i) {
		s = 0;
		for (j = 0; j < n; ++j) {
			if (i == j) continue;
			s = max(dist[i][j], s);
		}
		sum += s;
	}
	sum >>= 1;

	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			d[i][j] = dist[i][j] * max_t / sum;
	return d;
}

int** generateTimeWindows(int n, int close, int maxService, int seed /* = 5 */) {
	int a, b, i;
	int** d;

	srand(seed);
	d = new int*[n];
	for (i = 0; i < n; ++i)
		d[i] = new int[3];

	for (i = 0; i < n; ++i) {
		a = rand() % close;
		b = rand() % close;
		if (a > b) {
			d[i][0] = b;
			d[i][1] = a;
		} else {
			d[i][0] = a;
			d[i][1] = b;
			
		}
		d[i][2] = rand() % maxService;;
	}
	return d;
}

int* generateHourMultipler(int n, int variation /* = 15 */) {
	/* seed has beed set earlier !*/
	int* d = new int[n];
	for (int i = 0; i < n; ++i)
		d[i] = rand() % (20 * variation + 1) + 1000 - variation * 10;
	return d;
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

void printTab(int n, int m, int** t){
	int i, j;
	for (i = 0; i < n; ++i) {
		cout << "[";
		for (j = 0; j < m-1 ; ++j) cout << t[i][j] << ", ";
		cout << t[i][j] << "]\n";
	}
}
// void printTab(int n, int* t){ printTab(n,n,t); }





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
