// Node.cpp
#include<vector>
#include<iostream>

using namespace std;

class Node {
public:
	int i;
	int open;
	int close;
	int service;
	int k;
	vector<int> outputs;

	Node(int a = 0, int kk = 0) {
		i = a;
		k = k;
		open = close = service = 0;
	}
	Node(int a, int b, int c, int d = 0, int kk = 0) {
		i = a;
		open = b;
		close = c;
		service = d;
		k = kk;
	}
	~Node() { while(!outputs.empty()) outputs.pop_back(); }
	void addOutgoingArc(int a) {
		outputs.push_back(a); 
	}
	void print() {
		
		cout<<"N("<<i<<", "<<k<<") ["
			<<open<<", "
			<<close<<", "
			<<service
			<<"]";
		if (!outputs.empty()) {
			cout << " {";
			for (vector<int>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
				if (it != outputs.begin()) cout << " ";
					cout << *it;
			}
			cout << "}";
		}
		cout << endl;
	}
	void printT(){ cout<<"N("<<i<<") ["<<k<<"]"<<endl; }
};