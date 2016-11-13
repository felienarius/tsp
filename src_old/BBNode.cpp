struct BBNode {
	int cost;
	int travelTime;
	vector<int> seq;
	vector<int> kids;
	BBNode() {
		cost = travelTime = 0;
	}
	~BBNode() {
		while (!seq.empty()) seq.pop_back();
	}
	void print() {
		vector<int>::iterator it;

		std::cout << "cost = " << cost << " tt = " << travelTime << endl;
		if(!seq.empty()) {
			std::cout << "seq[";
			for (it = seq.begin(); it != seq.end(); ++it) {
				std::cout << " " << *it; 
			}
			std::cout << " ]\n";
		}
		if(!kids.empty()) {
			std::cout << "kid[";
			for (it = kids.begin(); it != kids.end(); ++it) {
				std::cout << " " << *it; 
			}
			std::cout << " ]\n";
		}
	}
};

struct CompareBBNodes {
	bool operator ()(const BBNode &node1, const BBNode &node2) {
		//kolejność - rosnąco
		if (node1.cost < node2.cost) return true;
		if (node1.cost > node2.cost) return false;

		if (node1.travelTime < node2.travelTime) return true;
		if (node1.travelTime > node2.travelTime) return false;
		return false;
	}
};

typedef priority_queue <BBNode, vector <BBNode>, CompareBBNodes> PriorityQueue;
