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
	vector<int> inputs;
	~Node(){
		while(!outputs.empty()) outputs.pop_back();
	}
	Node () {
		i = open = close = service = k = 0;
	}
	Node(int a){
 		i = a;
		open = close = service = k = 0; }
	// Node(int a, int b){ Node(a, 0, 0, 0); t = b; }
	Node(int a, int kk){
 		i = a;
 		k = kk;
		open = close = service = 0;
	}
	Node(int a, int b, int c, int d = 0, int kk = 0){
		i = a;
		open = b;
		close = c;
		service = d;
		k = kk;
	}
	// void setTime(int nt){ t = nt; }
	void addOutput(int a){ outputs.push_back(a); }
	void print() {
		cout<<"N("<<i<<", "<<k<<") ["
			<<open<<", "
			<<close<<", "
			<<service
			<<"]"<<endl;
	}
	void printT(){ cout<<"N("<<i<<") ["<<k<<"]"<<endl; }
};