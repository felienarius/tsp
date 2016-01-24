struct Data {
	int n;
	int **dist;
	int **t;
	int **windows;
	int *f;
	Data(int nn = 1) {
		n = nn;
	}
	~Data() {
		for (int i = 0; i < n; ++i) {
			delete []dist[i];
			delete []t[i];
			delete []windows[i];
		}
		delete []dist;
		delete []t;
		delete []windows;
		delete []f;
	}
};