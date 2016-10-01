// Branch and Bound bb.cpp
// #include <conio.h>
#include <iostream>
#include <queue>
#include "Graph.cpp"
// #include "Data.cpp"

class BranchAndBound {
  struct BBNode {
    int cost;
    int traveledTime;
    int visited;
    int k;
    vector<int> seq;
    vector<int> kids;
    BBNode() {
      cost = traveledTime = visited = k = 0;
    }
    ~BBNode() {
      while (!seq.empty()) seq.pop_back();
      while (!kids.empty()) kids.pop_back();
    }
  };
  class CompareBBNodes {
  public:
    bool operator()(const BBNode &node1, const BBNode &node2) {
      //kolejność - rosnąco
      if (node1.visited < node2.visited) return true;
      
      if (node1.cost < node2.cost) return true;
      if (node1.cost > node2.cost) return false;

      if (node1.traveledTime < node2.traveledTime) return true;
      if (node1.traveledTime > node2.traveledTime) return false;

      return false;
    }
  };
  priority_queue<BBNode, vector<BBNode>, CompareBBNodes> pq;
  Graph *g;
  vector<int> best_set;
  int best;
  int upperbound;
  int lowerbound;
  void propagate(BBNode n);
  int value(BBNode n);
  int bound(BBNode n, int next);
public:
  BranchAndBound(Graph *graph) {
    g = graph;
    best = upperbound = lowerbound = -1;
  }
  ~BranchAndBound(){
    while (!pq.empty()) {
      printBBN(pq.top());
      pq.pop();
    }
  }
  void printBBN(BBNode n);
  void work();
};

/* PRIVATE */
void BranchAndBound::propagate(BBNode n) {
  int i, count;
  vector<int> v;

  count = n.kids.size();
  for (i = 0; i < count; ++i) {
    BBNode bbn = BBNode();
    v = n.seq;
    v.push_back(n.kids[i]);
    bbn.seq = v;
    bbn.traveledTime = g->calcTraveledTime(v);
  }
}
int BranchAndBound::value(BBNode n) {
  int bound = n.cost;
  // int k = n.traveledTime;

  return bound;
}
int BranchAndBound::bound(BBNode n, int next) {
  return 0;
}
/* PUBLIC */
void BranchAndBound::printBBN(BBNode n) {
  vector<int>::iterator it;

  cout << "cost = " << n.cost << " tt = " << n.traveledTime
            << "visited = " << n.visited << endl;
  if(!n.seq.empty()) {
    std::cout << "seq[";
    for (it = n.seq.begin(); it != n.seq.end(); ++it) {
      std::cout << " " << *it; 
    }
    std::cout << " ]\n";
  }
  if(!n.kids.empty()) {
    std::cout << "kid[";
    for (it = n.kids.begin(); it != n.kids.end(); ++it) {
      std::cout << " " << *it; 
    }
    std::cout << " ]\n";
  }
}
void BranchAndBound::work() {
  BBNode n;
  unsigned int pls = 1;
  while(!pq.empty() && pls) {
    n = pq.top();
    pq.pop();
    if (n.visited == g->getN()) {
      if (value(n) < best) {
        best = value(n);
        best_set = n.seq;
      }
    } else {
      propagate(n);
      --pls;
    }
  }
}


// void addBBNode(PriorityQueue &pq, int cost, int travelTime, vector<int> seq, vector<int> kids) {
//  bb.cost = cost;
//  bb.travelTime = travelTime;
//  bb.seq = seq;
//  bb.kids = kids;
//  pq.push(bb);
// } 
// void bound(Data *d, vector<int> seq) {
//  int 
// }
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


// 36 x 5gr
// 78 x 1gr
// 40 x 2gr
// +30zl
// 199gr
