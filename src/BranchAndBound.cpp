using std::vector;

class CompareRoutes {
 public:
  
};
class BranchAndBound {
  std::priority_queue<Route, vector<Route>, std::less<Route>> pq;
  vector<unsigned int> best_set;
  unsigned int best;
  unsigned int upperbound;
  unsigned int lowerbound;
  void propagate(Route n);
  unsigned int value(Route n);
  unsigned int bound(Route n, int next);
public:
  BranchAndBound(Graph *graph) {
    best = upperbound = lowerbound = -1;
  }
  void work();
};

/* PRIVATE */
void BranchAndBound::propagate(Route r) {
  int i, count;
  vector<unsigned int>::iterator it;

  for (it = r.leftVertexes.begin(); i < count; ++i) {
    Route bbn = Route();
    v = n.seq;
    v.push_back(n.kids[i]);
    bbn.seq = v;
    bbn.traveledTime = g->calcTraveledTime(v);
  }
}
int BranchAndBound::value(const Route r) {
  return r.cost;
}
int BranchAndBound::bound(Route n, int next) {
  return 0;
}
/* PUBLIC */
void BranchAndBound::printBBN(Route n) {
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
  Route n;
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


// void addRoute(PriorityQueue &pq, int cost, int travelTime, vector<int> seq, vector<int> kids) {
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
