#include<stdio.h>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include "Graph.cpp"

using namespace std;

void printTab(int, int, int*);
void printTab(int, int*);
Graph* firstGraph(int, int*, int*);
Graph* toSecondGraph(Graph*);
Graph* toSecondGraph(int, int*, int*, int);
void toThirdGraph(Graph*);


int main() {
	// srand(time(0));
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

	int n2 = 4;
	int graf2[5][4][4]={
	{{0, 2, 1, 3},
	{1, 0, 1, 2},
	{1, 1, 0, 2},
	{1, 0, 0, 0}},
	{{0, 1, 1, 2},
	{1, 0, 0, 1},
	{1, 1, 0, 1},
	{1, 0, 0, 0}},
	{{0, 1, 0, 2},
	{1, 0, 0, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0}},
	{{0, 0, 0, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}},
	{{0, 0, 0, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}}};
	int okna2[4][3] = {{1, 7, 0}, {2, 4, 0}, {2, 3, 0}, {4, 6, 0}};

	Graph* g;
	Graph* g2;
	// g = firstGraph(n, graf[0], okna[0]);
	g = toSecondGraph(n2, &graf2[0][0][0], &okna2[0][0], 5);
	g->print();
	
	// g2 = toSecondGraph(g);
	// g2->printT();
	// delete g2;
	delete g;

	return 0;
}

Graph* fillGraph(int n, int* tab, int* w){
	// n - wymiar macierzy, gdzie
	// tab [n][n]
	// w   [n][3]
	int i;
	Graph* g = new Graph();
	cout<<"g.nodes size = "<<g->nodes.size()<<endl;
	cout<<"g.arcs size = "<<g->arcs.size()<<endl;
	for(i = 0; i < n; ++i){
		g->addNode(i);
		for(int j=0; j<n; ++j)
			if(i == j) continue;
			else g->addArc(i, j, tab[i*n + j]);
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
	Graph* g = new Graph(n);
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
	Graph* g = new Graph(graph->n);
	int n, i, j, p, aj, cost;

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
				aj = graph->nodes.at(graph->searchNode(j)).open;
				cost = graph->searchArcCost(i, j); // @TODO brak zmiennych czasów
				if(vj->t == max(aj, vi->t + cost))
				{
					//dodanie krawędzi @TODO brakuje kosztu czekania
					g->addArc(i, j, cost, vi->t);
				}
			}
		}
	}

 	// dodanie depo(-1)
 	g->addNode(-1);
	
	// dodawanie krawędzi z depo(-1) do 0 i odwrotnie
	p = graph->nodes.at(graph->searchNode(0)).close -
 		graph->nodes.at(graph->searchNode(0)).open + 1;
	for (i=0; i<p; ++i){
		j=0;
		for (vector<Arc>::iterator it = g->arcs.begin(); it != g->arcs.end(); ++it){
			if(!j&1 && it->start == 0 && it->k == i){ // wychodzący
				j = j|1;
				g->addArc(-1, 0, 0, i);
			}
			if(!j&2 && it->end == 0 && it->k == i){ // wchodzący
				j = j|2;
				g->addArc(0, -1, 0, i);
			}
		}
		if (j == 0) g->removeNode(0, i); // usunięcie 
	}
	return g;
}
Graph* toSecondGraph(int n, int* graph, int* windows, int t){
	Graph* g = new Graph(n);
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

void toThirdGraph(Graph* g){
	int i, k, min, plus, minus, in, out;
	vector<Node>::iterator nit;
	vector<Arc>::iterator ait;

	for (nit = g->nodes.begin(); nit != g->nodes.end(); ++nit)
		if (nit->i == 0) g->nodes.erase(nit);
	
	// przeniesienie krawędzi zamiast v0
	min = -1;
	k = 1;
	for (i = 1; i < g->n; ++i){
		for (ait = g->arcs.begin(); ait != g->arcs.end(); ++ait){
			if (ait->start == -1 && ait->end == i){
				if (min == -1) min = ait->cost;
				if (ait->cost < min){
					min = ait->cost;
					k = ait->k;
				}
			}
			if (ait->end == 0 && ait->start == i) g->addArc(i, -1, ait->cost, ait->k);
		}
		g->addArc(-1,i,min,k);
	}

	// redukcja zbędnych wierzchołków
	for (nit = g->nodes.begin(); nit != g->nodes.end(); ++nit){
		plus = minus = 0;
		i = nit->i;
		k = nit->t;
		for (ait = g->arcs.begin(); ait != g->arcs.end(); ++ait){
			if (ait->start == i  &&  ait->k == k){
				++plus;
				out = ait->end;
			}
			if (ait->end == i  &&  (ait->cost + ait->k == k)){
				++minus;
				in = ait->start;
			}
		}
		if (plus == 0 || minus == 0){
			if (plus + minus > 0) g->removeArcsOfNode(i, k);
			g->removeNode(i, k);
		}
		if (plus == 1 && minus == 1){
			if(in == -1  && out == -1){
				g->removeArcsOfNode(i, k);
				g->removeNode(i, k);
			}
		}
	}
}
