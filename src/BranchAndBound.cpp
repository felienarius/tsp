#include <queue>
#include <vector>
#include "Graph.cpp"

using std::vector;

struct BBNode {
  unsigned int cost;
  unsigned int traveledTime;
  unsigned int visited;
  unsigned int k;
  vector<unsigned int> seq;
  vector<unsigned int> kids;
  BBNode() {
    cost = traveledTime = visited = k = 0;
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
class BranchAndBound {
  priority_queue<BBNode, vector<BBNode>, CompareBBNodes> pq;
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
int BranchAndBound::value(const BBNode n) {
  return n.cost;
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
