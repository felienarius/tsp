/*
* C++ Program to Implement Traveling Salesman Problem using Nearest neighbour Algorithm
 */
#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<vector>
#include<ctime>
#include<cstdlib>

using namespace std;

class Arc{
public:
	int start;
	int end;
	int cost;
	Arc(){
		start = 0;
		end = 0;
		cost = 0;
	}
	Arc(int a, int b, int c = 0){
		start = a;
		end = b;
		cost = c;
	}
	void print(void){ cout<<"A["<<start<<", "<<end<<"] "; }
};

class Node{
public:
	int i;
	int open;
	int close;
	int service;
	Node(){
		i = open = close = service = 0;

	}
	Node(int a, int b = 0, int c = 0, int d = 0){
		i = a;
		open = b;
		close = c;
		service = d;
	}
};

class Graph{
public:
	vector<Node> nodes;
	vector<Arc> arcs;
	Graph(){};
	void addNode(Node n){ nodes.push_back(n); }
	void addNode(Node* n){ nodes.push_back(*n); }
	void addArc(Arc a){ arcs.push_back(a); }
	void addArc(Arc* a){ arcs.push_back(*a); }
};

/*
	Testy klas
*/
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

void printTab(int n, int* t){
	int i, j;
	for(i = 0; i < n; ++i){
		cout<<"[";
		for(j = 0; j < n - 1; ++j) cout<<t[i + j*n]<<", ";
		cout<<t[i + j*n]<<"]\n";
	}
}

/*
	main
*/

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
	int n = 7; // liczba wierzchołków
	int windows[7][3] ={{60*0, 60*MAX_WORKING_HOURS, 0},
						{60*1, 60*3, 10},
						{60*0, 60*2, 20},
						{60*4, 60*6, 15},
						{60*2, 60*3, 0},
						{60*2, 60*5, 7},
						{60*3, 60*4, 4}}; // [start, koniec, czas obsługi]
	int t[7][7];
	for (int i = 0; i < 7; ++i){
		for (int j = 0; j < 7; ++j){
			if (i == j){
				t[i][j] = 0;
			} else {
				if (j<i) t[i][j] = t[j][i];
				else t[i][j] = rand()%100+1;
			}
		}
	}
	printTab(n, &t[0][0]);
	Graph* g = fillGraph(n, &t[0][0], &windows[0][0]);

	cout<<"Test Arc "<<testArc()<<endl
		<<"Test Node "<<testNode()<<endl
		<<"Test Graph "<<testGraph()<<endl;

	return 0;
}