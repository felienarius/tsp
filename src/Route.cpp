class Route {
private:
  unsigned int k;
  long cost;
  unsigned int totalDistance;
  unsigned int travelTime;
  unsigned int startingDelay;
  unsigned int missedVertexes;
  vector<unsigned int> sequence;
public:
  Route(int a = 0);
  Route(Route *r);
}

Route::Route(int a = 0) {
  n = a;
  cost = 0L;
  k = distance = travelTime = workHours = delay = missed = 0;
}

Route::Route(Route r) {
  n = r.n;
  k = r.k;
  cost = r.cost;
  distance = r.distance;
  travelTime = r.travelTime;
  workHours = r.workHours;
  delay = r.delay;
  missed = r.missed;
  //TODO copy sequence
}