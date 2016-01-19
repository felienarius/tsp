// Arc.cpp

class Arc {
public:
	int start;	// wierzchołek początkowy
	int end;	// wierzchołek końcowy
	int dist;	// odległość
	int t;		// czas przejazdu
	int k;		// instancja czasu
	Arc() {
		start = end = dist = t = k = 0;
	}
	Arc(int i, int j, int d = 0, int tt = 0, int kk = 0) {
		start = i;
		end = j;
		dist = d;
		t = tt;
		k = kk;
	}
	void print(void) {
		cout << "A(" << start << ", " << end << ")["
			<< dist << ", " << t << ", " << k << "]\t";
	}
};