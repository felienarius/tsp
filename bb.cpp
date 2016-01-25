// Branch and Bound bb.cpp
// #include <conio.h>
#include <iostream>
#include <queue>
#include "Graph.cpp"

struct BBNode {
	int cost;
	int travelTime;
	vector<int> seq;
	vector<int> kids;
	BBNode() {
		cost = travelTime = 0;
	}
	~BBNode() {
		while (!seq.empty()) seq.pop_back();
	}
	void print() {
		vector<int>::iterator it;

		std::cout << "cost = " << cost << " tt = " << travelTime << endl;
		if(!seq.empty()) {
			std::cout << "seq[";
			for (it = seq.begin(); it != seq.end(); ++it) {
				std::cout << " " << *it; 
			}
			std::cout << " ]\n";
		}
		if(!kids.empty()) {
			std::cout << "kid[";
			for (it = kids.begin(); it != kids.end(); ++it) {
				std::cout << " " << *it; 
			}
			std::cout << " ]\n";
		}
	}
};

struct CompareBBNodes {
	bool operator ()(const BBNode &node1, const BBNode &node2) {
		//kolejność - rosnąco
		if (node1.cost < node2.cost) return true;
		if (node1.cost > node2.cost) return false;

		if (node1.travelTime < node2.travelTime) return true;
		if (node1.travelTime > node2.travelTime) return false;
		return false;
	}
};

typedef priority_queue < BBNode, vector < BBNode >, CompareBBNodes > PriorityQueue;

// void addBBNode(PriorityQueue &pq, int cost, int travelTime, vector<int> seq, vector<int> kids) {
// 	bb.cost = cost;
// 	bb.travelTime = travelTime;
// 	bb.seq = seq;
// 	bb.kids = kids;
// 	pq.push(bb);
// }

void branchAndBound(PriorityQueue &pq, Graph *g) {
	BBNode bb = BBNode();
	BBNode n;
	while (!pq.empty()) {
		BBNode parent;	
	}
} 

// void bound(Data *d, vector<int> seq) {
// 	int 
// }
// initialize(Q) //kolejka priorytetowa
// 	v ← korzeń drzewa //drzewo istnieje niejawnie
// 	best ← value(v)
// 	best items ← items(v)
// 	insert(Q, v)
// 	while !empty(Q) do
// 		v ← remove(Q)
// 		if bound(v) jest lepszy od best then
// 		for all dziecko u węzła v do
// 			if value(u) jest lepsza od best then
// 				best ← value(u)
// 				best items ← items(u)
// 			end if
// 			if bound(u) jest lepszy od best then
// 				insert(Q, u)
// 			end if
// 		end for
// 	end if
// end while


// 36 x 5gr
// 78 x 1gr
// 40 x 2gr
// +30zl
// 199gr