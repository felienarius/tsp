using std::vector;

class BranchAndBound {
 private:
  std::priority_queue<Route, vector<Route>, std::less<Route>> pq;
  std::unique_ptr<Route> best;
  unsigned int upperbound;
  unsigned int lowerbound;
  void propagate(const Route r);
  const unsigned int value(const Route r) const;
  unsigned int bound(Route r, int next);
public:
  BranchAndBound(const Route &r);
  void work();
};

BranchAndBound::BranchAndBound(const Route &r) {
  upperbound = lowerbound = -1;
}

void BranchAndBound::propagate(const Route r) {
  int i, count;
  vector<unsigned int>::iterator it = r.getLeftVertexes().begin();

  for (; it != r.getLeftVertexes().end(); ++it) {
    Route route = Route(r);
    route.addVertex(*it);
    // bbn.traveledTime = g->calcTraveledTime(v);
  }
}

const unsigned int BranchAndBound::value(const Route r) const {
  return r.getCost();
}

void BranchAndBound::work() {
  unsigned int pls = 1;
  while(!pq.empty() && pls) {
    std::unique_ptr<Route> r(new Route(pq.top()));
    pq.pop();
    if (r->isFinished()) {
      if (value(*r) < best->getCost()) {
        best = std::move(r);
      }
    } else {
      propagate(*r);
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
