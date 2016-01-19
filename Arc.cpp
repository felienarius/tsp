// Arc.cpp

class Arc{
public:
	int start;
	int end;
	int cost;
	int k; // czas
	Arc(){
		start = 0;
		end = 0;
		cost = 0;
		k = 0;
	}
	Arc(int a, int b, int c = 0, int ak = 0){
		start = a;
		end = b;
		cost = c;
		k = ak;
	}
	void print(void){ cout<<"A["<<start<<", "<<end<<"] "; }
	void printAll(void){ cout<<"A["<<start<<", "<<end<<", "<<cost<<"] "; }
};