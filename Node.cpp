// Node.cpp
#include<vector>
#include<iostream>

using namespace std;

class Node {
public:
  int i;
  int open;
  int close;
  int k;
  vector<int> outputs;

  Node(int a = 0, int kk = 0) {
    i = a;
    k = k;
    open = close = 0;
  }
  Node(int a, int b, int c, int kk = 0) {
    i = a;
    open = b;
    close = c;
    k = kk;
  }
  ~Node() { while(!outputs.empty()) outputs.pop_back(); }
  void addOutgoingArc(int a) {
    outputs.push_back(a);
  }
  void print() {
    cout << "N(" << i << ", " << k << ") ["
      << open << ", "
      << close << "]";
    if (!outputs.empty()) {
      cout << " {";
      for (vector<int>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
        if (it != outputs.begin())
          cout << " ";
        cout << *it;
      }
      cout << "}";
    }
    cout << endl;
  }
  void printT(){ cout<<"N("<<i<<") ["<<k<<"]"<<endl; }
};