#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "Graph.cpp"
#include "vns.cpp"

using namespace std;

struct Data {
	int n;
	int **dist;
	int **t;
	int **windows;
	int *f;
};

struct Route {
	int n;
	int k;
	int cost;
	int *seq;
};

void loadDistances(Graph *g, int n, int **dist);
void loadTimeWindows(Graph *g, int n, int **windows);
void loadTravelTimes(Graph *g, int n, int **times);
Graph *firstAuxiliaryGraph(Graph *g);

int loadATSP(string name, int ***dist);
int **calcTimeTravel(int n, int **dist, int max_t);
int **generateTimeWindows(int n, int close, int maxService, int seed = 5);
int *generateHourMultipler(int n, int variation = 15);
void printTab(int n, int m, int **t);
void printTab(int n, int *t);
void printData(Data *d);
void printRoute(Route *r);

Route *search(Data *d, int neighborhoods, int max_no_improv, int max_no_improv_ls);
Route *firstRoute(Data *d);

int main() {
	int workHours = 12;
	Data *d;
	Route *best;
	string pliki[13] = {"br17", "ft53", "ft70", "ftv33", "ftv35", "ftv38",
	 "ftv44", "ftv47", "ftv55", "ftv64", "ftv70", "p43", "ry48p"};
	
	d = new Data();
	d->n = loadATSP(pliki[0], &(d->dist));
	d->t = calcTimeTravel(d->n, d->dist, workHours * 60);
	d->windows = generateTimeWindows(d->n, workHours * 60, 10);
	d->f = generateHourMultipler(workHours);
	printData(d);
	
	int max_no_improv = 10;
	int max_no_improv_ls = 5;
	int neighborhoods = 3;
	// best = new Route();
	best = search(d, neighborhoods, max_no_improv, max_no_improv_ls);
	printRoute(best);
	// printTab(d->n - 1, best->seq);
	// cout<<"Done. Best Solution: c="<<best[CITIES]<<", v=";
	// for(int i=0; i<CITIES-1; ++i)
	// 	cout<<best[i]<<", ";
	// cout<<best[CITIES-1];
	// return 0;




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

	

	// delete []best;
	for (int i = 0; i < d->n; ++i) {
		delete []d->dist[i];
		delete []d->windows[i];
		delete []d->t[i];
	}
	delete []d->dist;
	delete []d->windows;
	delete []d->t;
	delete []d->f;
	delete []best->seq;

	return 0;
}


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

Graph* firstAuxiliaryGraph(Graph *graph){
	Graph* g = new Graph(graph->getN());
	// vertexes
	g->crossNodes(graph);
	// adding arcs
	g->connectNodes(graph);
	// adding Depot(-1)
	g->connectDepot(graph);
	return g;
}

int loadATSP(string name, int ***dist) {
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

	d[0][0] = 0;
	d[0][1] = close;
	d[0][2] = 0;
	for (i = 1; i < n; ++i) {
		a = rand() % close;
		b = rand() % close;
		if (a > b) {
			d[i][0] = b;
			d[i][1] = a;
		} else {
			d[i][0] = a;
			d[i][1] = b;
		}
		d[i][2] = rand() % (maxService + 1);
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

void printTab(int n, int m, int** t) {
	int i, j;
	for (i = 0; i < n; ++i) {
		cout << "[";
		for (j = 0; j < m-1 ; ++j) cout << t[i][j] << ", ";
		cout << t[i][j] << "]\n";
	}
}
void printTab(int n, int *t) {
	int i;
	cout << "[";
	for (i = 0; i < n - 1 ; ++i) cout << t[i] << ", ";
	cout << t[i] << "]\n";
}
void printData(Data *d) {
	cout << "\tDistances:\n";
	printTab(d->n, d->n, d->dist);
	cout << "\tTravelTimes:\n";
	printTab(d->n, d->n, d->t);
	cout << "\tTimeWindows:\n";
	printTab(d->n, 3, d->windows);
	cout << "\tHourMultipler:\n";
	printTab(d->windows[0][1]/60, d->f);
}
void printRoute(Route *r) {
	cout << "\tRoute seq(" << r->n << "):\n";
	printTab(r->n, r->seq);
	cout << "\tRoute cost: " << r->cost << endl;
	cout << "\tRoute k:    " << r->k << endl;
}


Route* search(Data *d, int neigh, int max_no_improv, int max_no_improv_ls) {
	srand(5);
	int iter, count, i, j;
	Route *best; //, *last_valid;
	/* perm[0] = cost  perm[1] = k  perm[2 - n-1]*/
	best = firstRoute(d);
	// printT(16, best->seq);
	// last_valid = new int[n + 3];
	// best[0] = cost(n, dist, t, win, f, &best[1], NULL);
	// printT(20, best);
	// iter = count = 0;
	// do {
	// 	for (i = 1; i < neigh; ++i) {
	// 		i = 1;
	// 		int *candidate = new int[n + 3];
	// 		copy(&best[0], &best[n + 3], &candidate[0]);
	// 		cout << "copy " << &best[0] << " " << &candidate[0] << endl;
	// 		// printT(20, candidate);
	// 		cout << "#3\n";
	// 		for (j = 0; j < i; ++j)
	// 			stochastic_two_opt(n, candidate);
	// 		cout << "THIS\n";
	// 		printT(20, candidate);
	// 		candidate[0] = cost(n, dist, t, win, f, &candidate[1], NULL);
	// 		cout << "THIS\n";
	// 		cout << "cost calcd ";
	// 		printT(20, candidate);
	// 		// candidate = local_search(n, candidate, dist, t, win, f, last_valid, max_no_improv, i);
	// 		cout<<" > iteration "<<iter+1<<", neigh="<<i<<", best="<<best[0]<<endl;
	// 		++iter;
	// 		if (candidate[0] < best[0]) {
	// 			count = 0;
	// 			delete []best;
	// 			best = candidate;
	// 			cout<<"New best, restarting neighborhood search."<<endl;
	// 		} else {
	// 			++count;
	// 			delete []candidate;
	// 		}
	// 	}
	// } while (count < max_no_improv);

	// delete []last_valid;
	return best;
}

Route *firstRoute(Data *d) {
	// int i, j, nodes, k, left;
	// int *sorted, *lnodes = new int[n - 1];
	Route *perm = new Route(); /* perm[0] = cost  perm[1] = k  perm[2 - n-1]*/
	perm->k = 0;
	perm->n = d->n - 1;
	perm->cost = 0;
	perm->seq = new int[perm->n];
	for (int i = 1; i < d->n; ++i)
		perm->seq[i - 1] = i;

	// cout << "WHAT";
	// sorted = sortTimeWindows(n, win); // [n-1] elem
	// nodes = 0;
	// left = 0;
	// j = sorted[nodes];
	// for (int a = 0; a < 14; ++a) {
	// 	for(i = 1; i < nodes + left + 2; ++i) {
	// 		k = calcRouteEndingAt(i, j, n, perm, t, win, f);
	// 		if (k <= win[j][1]) {
	// 			if (calcRouteStartingAt(i, j, k, n, perm, t, win, f) <= win[0][1]) {
	// 				insertNode(n, perm, i, j);
	// 				j = sorted[++nodes];
	// 				break;
	// 			}
	// 		}
	// 	}
	// }

	// for(i = 1; i < nodes + left + 2; ++i) {
	// 	k = calcRouteEndingAt(i, j, n, perm, t, win, f);
	// 	if (k <= win[j][1]) {
	// 		if (calcRouteStartingAt(i, j, k, n, perm, t, win, f) <= win[0][1]) {
	// 			insertNode(n, perm, i, j);
	// 			j = sorted[++nodes];
	// 			break;
	// 		}
	// 	}
	// }
	// cout << "WHAT";
	// delete []sorted;
	// delete []lnodes;
	return perm;
}