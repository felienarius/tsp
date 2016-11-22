#include "Graph.cpp"

using std::ifstream;
using std::string;
using std::vector;
using std::max;

using std::cout;
using std::endl;

class Config {
private:
  unsigned int index;
  unsigned int vertexCount = 0;
  unsigned int workHours = 4;
  unsigned int SEED = 5555;
  vector<vector<unsigned int>> distances; // n x n
  vector<vector<unsigned int>> times; // n x n
  vector<vector<unsigned int>> windows; // n x 3
  vector<unsigned int> timeDependency; // 12 x 1
  string pliki[14] = {"pp5", "br17", "ftv33", "ftv35", "ftv38", "p43",
    "ftv44", "ftv47", "ry48p", "ft53", "ftv55", "ftv64", "ftv70",
    "ft70" };
  void printVector(vector<vector<unsigned int>> vector);
  void printVector(vector<unsigned int> vector);
  void printTimeWindowsAverage();
public:
  Config();
  Config(unsigned int index);
  void loadATSP();
  void generateTravelTimes();
  void generateTimeWindows();
  void generateTimeDependency(unsigned int variation);
  void print();
  unsigned int getVertexCount() const;
  vector<vector<unsigned int>> getDistances();
  vector<vector<unsigned int>> getTimes();
  vector<vector<unsigned int>> getWindows();
  vector<unsigned int> getTimeDependency();
};

Config::Config() {
  index = 0;
}

Config::Config(const unsigned int index) {
  this->index = index;
}

void Config::loadATSP() {
  ifstream plik;
  string line, name;
  unsigned int i, j, value;

  name = pliki[index];
  // cout << "Loading " << name << ".atsp" << endl;
  plik.open((string("../atsp/") + name + string(".atsp")).c_str());
  if (!plik.good()) return;

  while (line.find("DIMENSION:")) plik >> line;
  plik >> vertexCount;

  while (line.find("EDGE_WEIGHT_SECTION")) plik >> line;
  
  distances.resize(vertexCount);
  for (i = 0; i < vertexCount; ++i) {
    distances[i].resize(vertexCount);
    for (j = 0; j < vertexCount; ++j) {
      plik >> distances[i][j];
    }
  }
  plik.close();
}

void Config::generateTravelTimes() {
  unsigned int i, j, s, max_t = workHours * 60;
  unsigned long sum = 0L;
  double rate;

  if (vertexCount == 0){
    cout << "Brak danych" << endl;
    return;
  }
  
  for (i = 0; i < vertexCount; ++i) {
    s = 0;
    for (j = 0; j < vertexCount; ++j) {
      if (i == j) continue;
      s = max(distances[i][j], s);
    }
    sum += s;
  }
  sum = sum*1/2;

  // cout << "Czas pracy: " << max_t << " minut " << endl;
  // cout << "Suma najdluzszych wyjazdow = " << sum << endl;

  rate = (double) max_t / sum;

  // cout << "Wspolczynnik wynosi " << rate << endl;

  times.resize(vertexCount);
  for (i = 0; i < vertexCount; ++i) {
    times[i].resize(vertexCount);
    for (j = 0; j < vertexCount; ++j) {
      times[i][j] = (unsigned int) distances[i][j] * rate;
    }
  }
}

void Config::generateTimeWindows() {
  unsigned int a, b, i = 0, close = workHours * 60;

  srand(SEED);

  windows.resize(vertexCount);
  windows[i].resize(2);
  windows[i][0] = 0;
  windows[i][1] = close;

  for (i = 1; i < vertexCount; ++i) {
    windows[i].resize(2);
    a = rand() % close;
    b = rand() % close;
    if (a > b) {
      windows[i][0] = b;
      windows[i][1] = a;
    } else {
      windows[i][0] = a;
      windows[i][1] = b;
    }
  }
}

void Config::generateTimeDependency(unsigned int variation = 20) {
  unsigned int i = 0;
  timeDependency.resize(workHours);
  srand(SEED);
  for (; i < workHours; ++i) {
    timeDependency[i] = rand() % (20 * variation + 1) + 1000 - variation * 10;
  }
}

void Config::printVector(vector<vector<unsigned int>> vector) {
  std::vector<std::vector<unsigned int>>::iterator row;
  for (row = vector.begin(); row != vector.end(); row++) {
    std::vector<unsigned int>::iterator col;
    cout << "[";
    for (col = row->begin(); col != row->end(); col++) {
      cout << *col << ", ";
    }
    cout << "]" << endl;
  }
}

void Config::printVector(vector<unsigned int> vector) {
    std::vector<unsigned int>::iterator it;
    cout << "[";
    for (it = vector.begin(); it != vector.end(); it++) {
      cout << *it << ", ";
    }
    cout << "]" << endl;
}

void Config::printTimeWindowsAverage() {
  unsigned int sum = 0, i = 0;
  for(; i < vertexCount; ++i) {
    sum += windows[i][1];
    sum -= windows[i][0];
  }
  cout << "Suma okien = " << sum << " Srednia = "
       << (double) sum / vertexCount << endl;
}

void Config::print() {
  cout << "\tDistances:\n";
  printVector(distances);
  cout << "\tTravelTimes:\n";
  printVector(times);
  cout << "\tTimeWindows:\n";
  printVector(windows);
  printTimeWindowsAverage();
  cout << "\tTimeDependency:\n";
  printVector(timeDependency);
}

unsigned int Config::getVertexCount() const {
  return vertexCount;
}

vector<vector<unsigned int>> Config::getDistances() {
  return distances;
}

vector<vector<unsigned int>> Config::getTimes() {
  return times;
}

vector<vector<unsigned int>> Config::getWindows() {
  return windows;
}

vector<unsigned int> Config::getTimeDependency() {
  return timeDependency;
}
