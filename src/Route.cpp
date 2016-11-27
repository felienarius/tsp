using std::vector;

class Route {
 private:
  unsigned int cost;
  unsigned int totalDistance;
  unsigned int travelTime;
  unsigned int startingDelay;
  vector<unsigned int> sequence;
  vector<unsigned int> leftVertexes;
 public:
  Route(const unsigned int vertexes);
  Route(const Route &r);
  const bool operator<(const Route &r) const;
  const unsigned int getCost() const;
  const bool isFinished() const;
  vector<unsigned int> getLeftVertexes() const;
  void addVertex(const unsigned int vertex);
  void print() const;
  std::string toString() const;
};
// const bool operator()(const Route &r1, const Route &r2);
// const bool operator==(const Route &r) const;

Route::Route(const unsigned int vertexes) {
  cost = totalDistance = travelTime = startingDelay = 0;
  for (unsigned int i = 1; i < vertexes; ++i) {
    leftVertexes.push_back(i);
  }
  sequence.push_back(vertexes);
}

Route::Route(const Route &r) {
  cost = r.cost;
  totalDistance = r.totalDistance;
  travelTime = r.travelTime;
  startingDelay = r.startingDelay;
  copy(r.sequence.begin(), r.sequence.end(), sequence.begin());
  copy(r.leftVertexes.begin(), r.leftVertexes.end(), leftVertexes.begin());
}

const bool Route::operator<(const Route &r) const {
  return (leftVertexes.size() < r.leftVertexes.size() ||
    leftVertexes.size() == leftVertexes.size() && cost < r.cost);
}

const unsigned int Route::getCost() const {
  return cost;
}

const bool Route::isFinished() const {
  return leftVertexes.empty();
}

vector<unsigned int> Route::getLeftVertexes() const {
  return leftVertexes;
}

void Route::addVertex(const unsigned int vertex) {
  vector<unsigned int>::iterator it = std::find(leftVertexes.begin(),
                                                leftVertexes.end(), vertex);
  if (it != leftVertexes.end())
    leftVertexes.erase(it);

}

void Route::print() const {
  std::cout << toString() << std::endl;
}

std::string Route::toString() const {
  std::stringstream ss;
  vector<unsigned int>::const_iterator it;

  ss << "R  (cost = " << cost << " tt = " << travelTime << " dist = "
     << totalDistance << "dealy = " << startingDelay << ")\n";
  if (!sequence.empty()) {
    ss << "seq[";
    for (it = sequence.begin(); it != sequence.end(); ++it) {
      ss << " " << *it; 
    }
    ss << " ]\n";
  }
  if(!leftVertexes.empty()) {
    ss << "kid[";
    for (it = leftVertexes.begin(); it != leftVertexes.end(); ++it) {
      ss << " " << *it; 
    }
    ss << " ]";
  }
  return ss.str();
}
