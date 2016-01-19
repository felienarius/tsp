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
	int t;
	vector<int> outputs;
	vector<int> inputs;
	Node(){
		i = open = close = service = t = 0;
	}
	Node(int a){
 		i = a;
		open = close = service = t = 0; }
	// Node(int a, int b){ Node(a, 0, 0, 0); t = b; }
	Node(int a, int b, int c, int d = 0, int ntime = 0){
		i = a;
		open = b;
		close = c;
		service = d;
		t = ntime;
	}
	Node(int a, int tt){
 		i = a;
 		t = tt;
		open = close = service = 0;
	}
	~Node(){
		while(!outputs.empty()) outputs.pop_back();
	}
	// void setTime(int nt){ t = nt; }
	void addOutput(int a){ outputs.push_back(a); }
	void print(){ cout<<"N("<<i<<") ["<<open<<", "<<close<<", "<<service<<"]"<<endl; }
	void printT(){ cout<<"N("<<i<<") ["<<t<<"]"<<endl; }
};