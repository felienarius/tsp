#include <iostream>
// #include <algorithm>
#include <vector>
#include "Arc.cpp"
using namespace std;

int main()
{
	vector<Arc> arcs;
	for (int i = 0; i < 167000; ++i) {
		// vector<Node>::iterator it;
		
		Arc arc = Arc(i, 5, 33, 876345, 33);
		arcs.push_back(arc);
		// for (it = nodes.begin(); it != nodes.end(); ++it)
		// 	if (it->i == i && it->k == k) {
		// 		it->addOutgoingArc(j);
		// 		break;
		// 	}
	}
	return 0;
}

// br17: 39
// ft53: 6905
// ft70: 38673
// ftv33: 1286
// ftv35: 1473
// ftv38: 1530
// ftv44: 1613
// ftv47: 1776
// ftv55: 1608
// ftv64: 1839
// ftv70: 1950
// ftv90: 1579
// ftv100: 1788
// ftv110: 1958
// ftv120: 2166
// ftv130: 2307
// ftv140: 2420
// ftv150: 2611
// ftv160: 2683
// ftv170: 2755
// kro124: 36230
// p43: 5620
// rbg323: 1326
// rbg358: 1163
// rbg403: 2465
// rbg443: 2720
// ry48p: 14422


// initialize(Q) //kolejka priorytetowa
//  v ← korzeń drzewa //drzewo istnieje niejawnie
//  best ← value(v)
//  best items ← items(v)
//  insert(Q, v)
//  while !empty(Q) do
//    v ← remove(Q)
//    if bound(v) jest lepszy od best then
//    for all dziecko u węzła v do
//      if value(u) jest lepsza od best then
//        best ← value(u)
//        best items ← items(u)
//      end if
//      if bound(u) jest lepszy od best then
//        insert(Q, u)
//      end if
//    end for
//  end if
// end while



	// // srand(time(0));
	// //parametry
	// // godziny pracy, 0 to pierwsza minuta pracy
	// int STARTING_HOUR = 7;
	// int STARTING_MIN = 0;
	// int ENDING_HOUR = 20;
	// int ENDING_MIN = 0;
	// int MAX_WORKING_HOURS = 10;
	// // GRAF
	// // int n = 6; // liczba wierzchołków
	// int graf[6][6]={{0, 7, 2, 11, 9, 6},
	// 				{7, 0, 15, 5, 8, 10},
	// 				{2, 15, 0, 4, 6, 3},
	// 				{11, 5, 4, 0, 3, 7},
	// 				{9, 8, 6, 3, 0, 7},
	// 				{6, 10, 3, 7, 7, 0}};
	// int okna[6][3]={{0, 40, 0},
	// 				{12, 30, 0},
	// 				{4, 9, 0},
	// 				{14, 26, 0},
	// 				{5, 20, 0},
	// 				{1, 13, 0}};

	// int n2 = 4;
	// int graf2[5][4][4]=
	// {{{0, 2, 1, 3},
	// {1, 0, 1, 2},
	// {1, 1, 0, 2},
	// {1, 0, 0, 0}},
	// {{0, 1, 1, 2},
	// {1, 0, 0, 1},
	// {1, 1, 0, 1},
	// {1, 0, 0, 0}},
	// {{0, 1, 0, 2},
	// {1, 0, 0, 1},
	// {0, 0, 0, 0},
	// {0, 0, 0, 0}},
	// {{0, 0, 0, 1},
	// {0, 0, 0, 0},
	// {0, 0, 0, 0},
	// {0, 0, 0, 0}},
	// {{0, 0, 0, 1},
	// {0, 0, 0, 0},
	// {0, 0, 0, 0},
	// {0, 0, 0, 0}}};
	// int okna2[4][3] = {{1, 7, 0}, {2, 4, 0}, {2, 3, 0}, {4, 6, 0}};

	// Graph* g;
	// Graph* g2;
	// // g = firstGraph(n, graf[0], okna[0]);
	// // g = toSecondGraph(n2, &graf2[0][0][0], &okna2[0][0], 5);
	// // g->print();
	
	// // g2 = toSecondGraph(g);
	// // g2->printT();
	// // delete g2;
	// // delete g;