// Branch and Bound bb.cpp
#include <iostream>
#include <queue>
// #include <conio.h>

struct BBNode {
	int cost;
	int travelTime;
	vector<int> seq;
	BBNode() {
		cost = travelTime = 0;
	}
	~BBNode() {
		while (!seq.empty()) seq.pop_back();
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

typedef priority_queue < BBNode, vector < BBNode >, CompareBBNodes > priorityQueue;

void addBBNode( priorityQueue & pq, int cost, int travelTime, vector<int> seq) {
	BBNode bb;
	bb.cost = cost;
	bb.travelTime = travelTime;
	bb.seq = seq;
	pq.push(bb);
}

// void 

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
