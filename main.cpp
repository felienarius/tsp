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
	long cost;
	int distance;
	int travelTime;
	int workHours;
	int delay;
	int missed;
	int *seq;
	Route(int a) {
		n = a;
		cost = 0L;
		k = distance = travelTime = workHours = delay = missed = 0;
	}
	Route() {
		n = k = distance = travelTime = workHours = delay = missed = 0;
		cost = 0L;
	}
};

int GODZINY_PRACY = 10;
int MAX_DELAY = 15;
double KOSZT_GODZINA = 15;
double NADGODZINA_KOSZT = 30;
double DROGA_KOSZT = 0.4;
double KOSZT_SPOZNIENIA = 0.5;
double KOSZT_NIEDOSTARCZENIA = 20; /*per node*/

void loadDistances(Graph *g, int n, int **dist);
void loadTimeWindows(Graph *g, int n, int **windows);
void loadTravelTimes(Graph *g, int n, int **times);
Graph *firstAuxiliaryGraph(Graph *g);

int loadATSP(string name, int ***dist);
int **calcTimeTravel(int n, int **dist, int max_t);
int **generateTimeWindows(int n, int close, int maxService, int seed = 5);
int *generateHourMultipler(int n, int variation = 15, int seed = 5);
void printTab(int n, int m, int **t);
void printTab(int n, int *t);
void printData(Data *d);
void printRoute(Route *r);

Route *search(Data *d, int neighborhoods, int max_no_improv, int max_no_improv_ls);
/* funcion set for first route */
Route *firstRoute(Data *d);
int *sortTimeWindows(int n, int **win);
int calcRouteTravelTimeEndingAt(Data *d, Route *r, int index, int node);
int calcRouteTravelTimeEndingAt(Data *d, int *seq, int index, int node);
int calcRouteTravelTimeStartingAt(Data *d, Route *r, int index , int node , int k);
void insertNode(Route *r, int index, int node);

void calcRoute(Data *d, Route *r);
long calcRouteCost(Route *r);

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
	sum = sum*1/2;

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

int* generateHourMultipler(int n, int variation /* = 15 */, int seed /* = 5 */) {
	int i;

	srand(seed);
	int* d = new int[n];
	for (i = 0; i < n; ++i) {
		d[i] = rand() % (20 * variation + 1) + 1000 - variation * 10;
	}
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
	cout << "  k, cost, distance, workHours, delay, missed\n";
	cout << "R[" << r->k << ", "
		<< r->cost << ", "
		<< r->distance << ", "
		<< r->workHours << ", "
		<< r->delay << ", "
		<< r->missed << "]\n";
}


Route* search(Data *d, int neigh, int max_no_improv, int max_no_improv_ls) {
	srand(5);
	int iter, count, i, j;
	Route *r; //, *last_valid;
	/* perm[0] = cost  perm[1] = k  perm[2 - n-1]*/
	r = firstRoute(d);
	// printT(16, best->seq);
	// last_valid = new int[n + 3];
	// calcRoute(d, r);
	// calcRouteCost(r);
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
	return r;
}

Route *firstRoute(Data *d) {
	int i, j, nodes, k, /*left, */sum, best, bestIndex;
	int *sorted;/*, *lnodes;*/
	Route *r;

	r = new Route(d->n - 1);
	// lnodes = new int[r->n];
	r->seq = new int[r->n];
	for (i = 0; i < r->n; ++i)
		r->seq[i] = 0;
	sorted = sortTimeWindows(r->n, d->windows);
	nodes = 0;
	// left = 0;
	cout << "\tSorted:\n";
	printT(r->n, sorted);

	cout << "\tFirstRoute:\n";
	while(nodes < r->n) {
		j = sorted[nodes];
		bestIndex = nodes + 1;
		for(i = 0; i < nodes + 1; ++i) {
			printT(r->n, r->seq);
			k = calcRouteTravelTimeEndingAt(d, r, i, j);
			sum = calcRouteTravelTimeStartingAt(d, r, i, j, max(k, d->windows[j][0]));
			if (k <= d->windows[j][1])
				if (sum <= d->windows[0][1])
					if (bestIndex > nodes || sum < best) {
						best = sum;
						bestIndex = i;
					}
			if (bestIndex > nodes) {
				if (i == 0)
					best = sum;
				if (sum < best) {
					best = sum;
					bestIndex = nodes + 1 + i;
				}
			}
		}

		cout << "\t\tbestIndex = " << bestIndex << "(" << best << ")\n";
		insertNode(r, bestIndex % (nodes + 1), j);
		++nodes;
	}
	delete []sorted;
	return r;
}

int *sortTimeWindows(int n, int **win) {
	int i, j, tmp;
	int *mid = new int[n];
	int *sort = new int[n];
	for (i = 0; i < n; ++i) {
		mid[i] = (win[i + 1][0] + win[i + 1][1]) / 2;
		sort[i] = i + 1;
	}
	for (i = 0; i < n - 1; ++i) {
		for (j = 0; j < n - 1; ++j) {
			if (i == j) continue;
			if (mid[i] < mid[j]) {
				swap(mid[i], mid[j]);
				swap(sort[i], sort[j]);
			}
		}	
	}
	delete []mid;
	return sort;
}

int calcRouteTravelTimeEndingAt(Data *d, Route *r, int index, int node) {
	int a, b, i, j, sum, s;
	cout << "\tcalcRouteTravelTimeEndingAt(" << index << ", " << node << ")\n";
	cout << "[";
	for (i = -1; i < index; ++i) {
		if (i == -1) cout << "0";
		else cout << r->seq[i];
		cout << ", ";
	}
	cout << node <<"]\n";

	cout << "sum = " << r->k << endl;
	sum = r->k;
	a = -1;
	b = 0;
	do {
		if (a == -1) i = 0;
		else i = r->seq[a];

		if (b == index) j = node;
		else j = r->seq[b];

		cout << "sum += w[" << i << "][2] += " << d->windows[i][2] << " = ";
		sum += d->windows[i][2];
		cout << sum << endl;
		s = d->t[i][j] * d->f[min(sum, d->windows[0][1] - 1)/60]/1000;
		cout << "[" << i << "] -> " << s <<" -> [" << j << "]\n";
		cout << "sum = max(" << s << " + " << sum << ", " << d->windows[j][0] << ") = ";
		sum = max(s + sum, d->windows[j][0]);
		cout << sum << endl;
		++a;++b;
	} while (b <= index);
	// cout << "sum = " << sum << endl;
	cout << "calcRouteTravelTimeEndingAt(" << index << ", " << node << ")\n";
	return sum;
}
int calcRouteTravelTimeEndingAt(Data *d, int *seq, int index, int node) {
	int sum;
	Route *r = new Route(d->n - 1);
	r->seq = seq;
	// cout << "##";
	// printT(index, seq);
	sum = calcRouteTravelTimeEndingAt(d, r, index, node);
	r->seq = NULL;
	delete r;
	return sum;
}

int calcRouteTravelTimeStartingAt(Data *d, Route *r, int index , int node , int k) {
	int a, b, i, j, sum, s;

	cout << "calcRouteTravelTimeStartingAt(" << index << ", " << node << " )\n";
	cout << "[";
	for (i = index - 1; r->seq[i] != 0; ++i) {
		if (i == index - 1) cout << node;
		else cout << r->seq[i];
		cout << ", ";
	}
	cout << 0 <<"]\n";

	cout << "sum = k = " << k << endl;
	sum = k;
	a = index - 1;
	b = index;
	do {
		if (a == index - 1) i = node;
		else i = r->seq[a];

		if (b == r->n) j = 0;
		else j = r->seq[b];

		cout << "sum += w[" << i << "][2] += " << d->windows[i][2] << " = ";
		sum += d->windows[i][2];
		cout << sum << endl;

		s = d->t[i][j] * d->f[min(sum, d->windows[0][1] - 1)/60]/1000;
		cout << "[" << i << "] -> " << s <<" -> [" << j << "]\n";
		cout << "sum += max(" << s << ", " << d->windows[j][0] << ") = ";
		sum = max(s + sum, d->windows[j][0]);
		cout << sum << endl;
		++a;++b;
	} while (j != 0);
	cout << "\tcalcRouteTravelTimeStartingAt(" << index << ", " << node << ")\n";
	return sum;
}

void insertNode(Route *r, int index, int node) {
	copy(r->seq + index, r->seq + r->n - 1, r->seq + index + 1);
	r->seq[index] = node;
}

void calcRoute(Data *d, Route *r) {
	int distance, delay, missed, close, k, i, j, a, b, cost;

	distance = delay = missed = cost = 0;
	k = r->k;
	a = - 1;
	b = 0;
	do {
		if (a == -1) i = 0;
		else i = r->seq[a];

		if (b == r->n) j = 0;
		else j = r->seq[b];

		k += d->windows[i][2];
		k += max(d->t[i][j] * d->f[min(k, d->windows[0][1])/60 - 1]/1000, d->windows[j][0]);
		distance += d->dist[i][j];
		close = d->windows[j][1];
		if (k > close) {
			if (k > close + MAX_DELAY) ++missed;
			delay += k - close;
		}
		++a;++b;
	} while (j != 0);

	r->workHours = 1 + (k - r->k - 1) / 60;
	r->travelTime = k;
	r->distance = distance;
	r->delay = delay;
	r->missed = missed;
	calcRouteCost(r);
}

long calcRouteCost(Route *r) {
	long cost =  KOSZT_GODZINA * min(r->workHours, GODZINY_PRACY) +
		NADGODZINA_KOSZT * max(0, r->workHours - GODZINY_PRACY) +
		DROGA_KOSZT * r->distance + 
		KOSZT_SPOZNIENIA * r->delay +
		KOSZT_NIEDOSTARCZENIA * r->missed;
	r->cost = cost;
	return cost;
}
