#include <vector>

using std::vector;

class Route {
 private:
  unsigned int k;
  long cost;
  unsigned int totalDistance;
  unsigned int travelTime;
  unsigned int startingDelay;
  unsigned int leftVertexes;
  vector<unsigned int> sequence;
 public:
  Route(unsigned int a = 0);
  Route(Route &r);
};

Route::Route(unsigned int a) {
  cost = 0L;
  k = totalDistance = travelTime = startingDelay = leftVertexes = 0;
}

Route::Route(Route &r) {
  k = r.k;
  cost = r.cost;
  totalDistance = r.totalDistance;
  travelTime = r.travelTime;
  startingDelay = r.startingDelay;
  leftVertexes = r.leftVertexes;
  sequence = r.sequence;
}