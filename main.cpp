// #include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "vns.cpp"
#include "bb.cpp"
#include "Route.cpp"

// using namespace std;

const int GODZINY_PRACY = 10;
const int MAX_DELAY = 15;
const double KOSZT_GODZINA = 15;
const double NADGODZINA_KOSZT = 30;
const double DROGA_KOSZT = 0.4;
const double KOSZT_SPOZNIENIA = 1.0;
const double KOSZT_NIEDOSTARCZENIA = 20;

Graph *firstAuxiliaryGraph(Data *d);

int loadATSP(string name, int ***dist);
int **calcTimeTravel(int n, int **dist, int max_t);
int **generateTimeWindows(int n, int close, int maxService, int seed = 5);
int *generateHourMultipler(int n, int variation = 15, int seed = 5);
void printTab(int n, int m, int **t);
void printTab(int n, int *t);
void printData(Data *d);
void printRoute(Route *r);

Route *search(Data *d, int neighborhoods, int max_no_improv, int max_no_improv_ls);
Route *firstRoute(Data *d);
int *sortTimeWindows(int n, int **win);
int calcRouteTravelTimeEndingAt(Data *d, Route *r, int index, int node);
int calcRouteTravelTimeStartingAt(Data *d, Route *r, int index , int node , int k);
void insertNode(Route *r, int index, int node);

void calcRoute(Data *d, Route *r);
long calcRouteCost(Route *r);
void stochastic_two_opt(Route *r);
void local_search(Data *d, Route *best, int max_no_improv, int nhood);
void calcBestStartTime(Data *d, Route *route);

int main() {
  int workHours = 12;
  Data *d;
  string pliki[13] = {"br17", "ftv33", "ftv35", "ftv38", "p43", "ftv44",
    "ftv47", "ry48p", "ft53", "ftv55", "ftv64", "ftv70", "ft70"};


  d = new Data();
  d->n = loadATSP(pliki[0], &(d->dist));
  d->t = calcTimeTravel(d->n, d->dist, workHours * 60);
  d->windows = generateTimeWindows(d->n, workHours * 60, 10);
  d->f = generateHourMultipler(workHours, 20);
  // printData(d);
  
  // int max_no_improv = 1000;
  // int max_no_improv_ls = 150;
  // int neighborhoods = min(d->n - 2, 22);

  // best = search(d, neighborhoods, max_no_improv, max_no_improv_ls);
  // printRoute(best);

  // int n = 6; // liczba wierzchołków
  // Graph* dg = new Graph();
  Graph* tg;

  // dg->loadData(d);
  tg = firstAuxiliaryGraph(d);
  tg->printShort();
  tg->convertToSecondAuxiliaryGraph(d);
  tg->printShort();
  // PriorityQueue pq;
  // branchAndBound(pq, tg);

  // cout << tg->getArcsCount() << endl;
  delete tg;
  // delete dg;
  // delete []best;
  
  delete d;
  // delete best;
  return 0;
}

Graph *firstAuxiliaryGraph(Data *d) {
  Graph *g = new Graph();
  // vertexes
  g->crossNodes(d);
  // adding arcs
  g->connectNodes(d);
  // adding Depot(-1)
  g->connectDepot(d);
  return g;
}

int loadATSP(string name, int ***dist) {
  ifstream plik;
  string line;
  int i, j, n;
  int** d;

  plik.open((string("atsp/") + name + string(".atsp")).c_str());
  if (!plik.good()) return 0;

  while (line.find("DIMENSION:")) plik >> line;
  plik >> n;
  d = new int*[n];
  for (i = 0; i < n; ++i)
    d[i] = new int[n];
  while (line.find("EDGE_WEIGHT_SECTION")) // getline (plik, line) ) {//pętla nieskończona
    plik >> line;
  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
      plik >> d[i][j];
  plik.close();
  *dist = d;
  return n;
}

int** calcTimeTravel(int n, int** dist, int max_t) {
  int i, j, sum, s;
  int** d;

  d = new int*[n];
  for (i = 0; i < n; ++i) {
    d[i] = new int[n];
  }

  sum = 0;
  for (i = 0; i < n; ++i) {
    s = 0;
    for (j = 0; j < n; ++j) {
      if (i == j) continue;
      s = max(dist[i][j], s);
    }
    sum += s;
  }
  cout << "suma w rzędzie " << sum << endl;
  sum = sum*1/2;

  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
      d[i][j] = dist[i][j] * max_t / sum;
  return d;
}

int** generateTimeWindows(int n, int close, int maxService, int seed /* = 5 */) {
  int a, b, i;
  int** d;

  srand(seed);
  d = new int*[n];
  for (i = 0; i < n; ++i)
    d[i] = new int[3];

  d[0][0] = 0;
  d[0][1] = close;
  d[0][2] = 0;
  for (i = 1; i < n; ++i) {
    a = rand() % close;
    b = rand() % close;
    if (a > b) {
      d[i][0] = b;
      d[i][1] = a;
    } else {
      d[i][0] = a;
      d[i][1] = b;
    }
    d[i][2] = rand() % (maxService + 1);
  }
  return d;
}

int* generateHourMultipler(int n, int variation /* = 15 */, int seed /* = 5 */) {
  int i;

  srand(seed);
  int* d = new int[n];
  for (i = 0; i < n; ++i) {
    d[i] = rand() % (20 * variation + 1) + 1000 - variation * 10;
  }
  return d;
}

void printTab(int n, int m, int** t) {
  int i, j;
  for (i = 0; i < n; ++i) {
    cout << "[";
    for (j = 0; j < m-1 ; ++j) cout << t[i][j] << ", ";
    cout << t[i][j] << "]\n";
  }
}
void printTab(int n, int *t) {
  int i;
  cout << "[";
  for (i = 0; i < n - 1 ; ++i) cout << t[i] << ", ";
  cout << t[i] << "]\n";
}
void printData(Data *d) {
  cout << "\tDistances:\n";
  printTab(d->n, d->n, d->dist);
  cout << "\tTravelTimes:\n";
  printTab(d->n, d->n, d->t);
  cout << "\tTimeWindows:\n";
  printTab(d->n, 3, d->windows);
  cout << "\tHourMultipler:\n";
  printTab(d->windows[0][1]/60, d->f);
}
void printRoute(Route *r) {
  cout << "\tRoute seq(" << r->n << ", " << r->k << "):\n";
  printTab(r->n, r->seq);
  cout << "  cost = " << r->cost << endl
    << "  distance = " << r->distance << endl
    << "  travelTime = " << r->travelTime << endl
    << "  workHours = " << r->workHours << endl
    << "  delay = " << r->delay << endl
    << "  missed = " << r->missed << endl;
}


Route* search(Data *d, int neigh, int max_no_improv, int max_no_improv_ls) {
  int count, i, j;
  Route *best, *candidate;
  
  srand(5);
  best = firstRoute(d);
  printRoute(best);
  count = 0;
  do {
    for (i = 1; i <= neigh; ++i) {
      candidate = new Route(best);
      for (j = 0; j < i; ++j)
        stochastic_two_opt(candidate);
      calcRoute(d, candidate);
      local_search(d, candidate, max_no_improv, i);
      // cout << " > Iteration " << ++iter << ", neigh = " << i << ", best = " << best->cost << endl;
      // ++iter;
      if (candidate->cost < best->cost) {
        count = 0;
        delete best;
        best = candidate;
        cout << "New best, restarting neighborhood search." << endl;
        printRoute(best);
      } else {
        ++count;
        delete candidate;
      }
    }
  } while (count < max_no_improv);

  calcBestStartTime(d, best);
  calcRoute(d, best);
  printRoute(best);
  return best;
}

Route *firstRoute(Data *d) {
  int i, j, nodes, k, sum, best, bestIndex;
  int *sorted;
  bool missed;
  Route *r;

  r = new Route(d->n - 1);
  r->seq = new int[r->n];
  for (i = 0; i < r->n; ++i)
    r->seq[i] = 0;
  sorted = sortTimeWindows(r->n, d->windows);
  nodes = 0;
  while(nodes < r->n) {
    j = sorted[nodes];
    bestIndex = 0;
    missed = 1;
    for(i = 0; i < nodes + 1; ++i) {
      k = calcRouteTravelTimeEndingAt(d, r, i, j);
      sum = calcRouteTravelTimeStartingAt(d, r, i, j, max(k, d->windows[j][0]));
      if (k <= d->windows[j][1])
        if (sum <= d->windows[0][1])
          if (missed || sum < best) {
            best = sum;
            bestIndex = i;
            missed = 0;
          }
      if (missed)
        if (sum < best || i == 0) {
          best = sum;
          bestIndex = i;
        }
    }
    insertNode(r, bestIndex, j);
    ++nodes;
  }
  delete[] sorted;
  calcRoute(d, r);
  return r;
}

int *sortTimeWindows(int n, int **win) {
  int i, j;
  int *mid = new int[n];
  int *sort = new int[n];
  for (i = 0; i < n; ++i) {
    mid[i] = (win[i + 1][0] + win[i + 1][1]) / 2;
    sort[i] = i + 1;
  }
  for (i = 0; i < n - 1; ++i) {
    for (j = 0; j < n - 1; ++j) {
      if (i == j) continue;
      if (mid[i] < mid[j]) {
        swap(mid[i], mid[j]);
        swap(sort[i], sort[j]);
      }
    } 
  }
  delete[] mid;
  return sort;
}

int calcRouteTravelTimeEndingAt(Data *d, Route *r, int index, int node) {
  int a, b, i, j, sum;

  sum = r->k;
  a = -1;
  b = 0;
  do {
    if (a == -1) i = 0;
    else i = r->seq[a];

    if (b == index) j = node;
    else j = r->seq[b];

    sum += d->windows[i][2];
    sum += d->t[i][j] * d->f[min(sum, d->windows[0][1] - 1)/60]/1000;
    sum = max(sum, d->windows[j][0]);
    ++a;++b;
  } while (b <= index);
  return sum;
}

int calcRouteTravelTimeStartingAt(Data *d, Route *r, int index , int node , int k) {
  int a, b, i, j, sum;

  sum = k;
  a = index - 1;
  b = index;
  do {
    if (a == index - 1) i = node;
    else i = r->seq[a];

    if (b == r->n) j = 0;
    else j = r->seq[b];

    sum += d->windows[i][2];

    sum += d->t[i][j] * d->f[min(sum, d->windows[0][1] - 1)/60]/1000;
    sum = max(sum, d->windows[j][0]);
    ++a;++b;
  } while (j != 0);
  return sum;
}

void insertNode(Route *r, int index, int node) {
  copy_backward(r->seq + index, r->seq + r->n - 1, r->seq + r->n);
  r->seq[index] = node;
}

void calcRoute(Data *d, Route *r) {
  int distance, delay, missed, close, k, i, j, a, b;

  distance = delay = missed = 0;
  k = r->k;
  a = -1;
  b = 0;
  do {
    if (a == -1) i = 0;
    else i = r->seq[a];

    if (b == r->n) j = 0;
    else j = r->seq[b];

    k += d->windows[i][2];
    k += d->t[i][j] * d->f[min(k, d->windows[0][1] - 1)/60]/1000;
    k = max(k, d->windows[j][0]);
    distance += d->dist[i][j];
    close = d->windows[j][1];
    if (k > close) {
      if (k > close + MAX_DELAY) ++missed;
      delay += k - close;
    }
    ++a;++b;
  } while (j != 0);

  r->workHours = 1 + (k - r->k - 1) / 60;
  r->travelTime = k;
  r->distance = distance;
  r->delay = delay;
  r->missed = missed;
  calcRouteCost(r);
}

long calcRouteCost(Route *r) {
  long cost = KOSZT_GODZINA * min(r->workHours, GODZINY_PRACY) +
    NADGODZINA_KOSZT * max(0, r->workHours - GODZINY_PRACY) +
    DROGA_KOSZT * r->distance + 
    KOSZT_SPOZNIENIA * r->delay +
    KOSZT_NIEDOSTARCZENIA * r->missed;
  r->cost = cost;
  return cost;
}

void stochastic_two_opt(Route *r) {
  int index1, index2;
  int *e = new int[3];
  index1 = rand() % r->n;
  index2 = rand() % r->n;
  e[0] = index1;
  e[1] = (index1 + 1) % r->n;
  e[2] = (index1 + r->n - 1) % r->n;
  while (index2 == e[0] || index2 == e[1] || index2 == e[2])
    index2 = rand() % r->n;
  delete []e;
  if (index1 > index2) {
    for (;index1 > index2; --index1, ++index2)
      swap(r->seq[index1], r->seq[index2]);
  } else {
    for (;index1 < index2; ++index1, --index2)
      swap(r->seq[index1], r->seq[index2]);
  }
}
void local_search(Data *d, Route *best, int max_no_improv, int nhood) {
  int i, count = 0;
  Route *candidate;
  do {
    candidate = new Route(best);
    for (i = 0; i < nhood; ++i)
      stochastic_two_opt(candidate);
    calcRoute(d, candidate);
    if (candidate->cost < best->cost) {
      count = 0;
      delete best;
      best = candidate;
    } else {
      ++count;
      delete candidate;
    }
  } while (count < max_no_improv);
}

void calcBestStartTime(Data *d, Route *route) {
  int i, close, mn, mx;
  Route *r;
  r = new Route(route);
  printRoute(r);
  cin >> i;

  close = d->windows [route->seq[0]] [1];

  for (i = 0; i <= close; i=i+5) {
    r->k = i;
    calcRoute(d, r);
    if (r->cost < route->cost) {
      route->k = i;
      route->cost = r->cost;
    }
  }
  mn = max(route->k - 4, 0);
  mx = min(route->k + 4, close);
  for (i = mn; i <= mx ; ++i) {
    r->k = i;
    calcRoute(d, r);
    if (r->cost <= route->cost) {
      route->k = i;
      route->cost = r->cost;
    }
  }
  delete r;
}
