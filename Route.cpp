struct Route {
	int n;
	int k;
	long cost;
	int distance;
	int travelTime;
	int workHours;
	int delay;
	int missed;
	int *seq;
	Route(int a = 0) {
		n = a;
		cost = 0L;
		k = distance = travelTime = workHours = delay = missed = 0;
	}
	Route(Route *r) {
		n = r->n;
		k = r->k;
		cost = r->cost;
		distance = r->distance;
		travelTime = r->travelTime;
		workHours = r->workHours;
		delay = r->delay;
		missed = r->missed;
		seq = new int[n];
		copy(r->seq, r->seq + n, seq);
	}
	~Route() {
		delete []seq;
	}
};