using std::vector;

class Route {
 private:
  long cost;
  unsigned int totalDistance;
  unsigned int travelTime;
  unsigned int startingDelay;
  vector<unsigned int> sequence;
  vector<unsigned int> leftVertexes;
 public:
  // const bool operator()(const Route &r1, const Route &r2);
  const bool operator<(const Route &r) const;
  // const bool operator==(const Route &r) const;
  Route(unsigned int vertexes);
  Route(Route &r);
};
const bool Route::operator<(const Route &r) const {
  return (leftVertexes.size() < r.leftVertexes.size() ||
    leftVertexes.size() == leftVertexes.size() && cost < r.cost);
}
// const bool Route::operator==(const Route &r) const {
//   //kolejność - rosnąco
//   if (leftVertexes.size() < leftVertexes.size()) return true;
  
//   if (node1.cost < node2.cost) return true;
//   if (node1.cost > node2.cost) return false;

//   if (node1.traveledTime < node2.traveledTime) return true;
//   if (node1.traveledTime > node2.traveledTime) return false;

//   return false;
// }

Route::Route(unsigned int vertexes) {
  cost = 0L;
  totalDistance = travelTime = startingDelay = 0;
  for (unsigned int i = 1; i < vertexes; ++i) {
    leftVertexes.push_back(i);
  }
  sequence.push_back(vertexes);
}

Route::Route(Route &r) {
  cost = r.cost;
  totalDistance = r.totalDistance;
  travelTime = r.travelTime;
  startingDelay = r.startingDelay;
  leftVertexes = r.leftVertexes;
  sequence = r.sequence;
}