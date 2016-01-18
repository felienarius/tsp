#include<stdio.h>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<cmath>

using namespace std;

const static int CITIES = 52;

void reverse(int* perm, int c1, int c2){
	int i1, i2;
	i1 = i2 = 0;
	for(int i=0; i<CITIES; ++i){
		if(perm[i] == c1) i1 = i;
		if(perm[i] == c2) i2 = i;
	}
	if (i1 > i2) swap(i1, i2);
	for(;i1 < i2; ++i1, --i2) swap(perm[i1], perm[i2]);
}

int euc_2d(int* c1, int* c2){
	return sqrt((c1[0] - c2[0])*(c1[0] - c2[0]) + (c1[1] - c2[1])*(c1[1] - c2[1]));
}

int cost(int* perm, int* cities){
	int distance = 0;
	int c1, c2;
	for(int i=0; i<CITIES; ++i){
		c1 = perm[i];
		c2 = perm[(i+1)%CITIES];
		distance += euc_2d(&cities[c1*2], &cities[c2*2]);
	}
	return distance;
}

int* random_permutation(int* cities){
	int* perm = new int[CITIES+1];
	int r;

	for(int i=0; i<CITIES; ++i) perm[i] = i;
	for(int i=0; i<CITIES; ++i){
		r = rand()%(CITIES-1)+1;
		swap(perm[r], perm[i]);
	}
	return perm;
}

void stochastic_two_opt(int* perm){
	int tmp;
	int c1, c2;
	int e[3];
	c1=rand()%CITIES;
	c2=rand()%CITIES;
	e[0] = c1;
	e[1] = (e[0]+1)%CITIES;
	e[2] = (e[0] + CITIES - 1)%CITIES;
	while(c2 == e[0] || c2 == e[1] || c2 == e[2]) c2=rand()%CITIES;
	if(c2 < c1){
		tmp = c1;
		c1 = c2;
		c2 = tmp;
	}
	reverse(&perm[0], c1, c2);
}

int* local_search(int* best, int* cities, int max_no_improv, int nhood){
	int count = 0;
	do{
		int* candidate = new int[CITIES+1];
		copy(&best[0], &best[CITIES+1], &candidate[0]);
		for(int i=0; i<nhood; ++i) stochastic_two_opt(candidate);
		candidate[CITIES] = cost(candidate, cities);
		if(candidate[CITIES] < best[CITIES]){
			count = 0;
			delete []best;
			best = candidate;
		} else{
			++count;
			delete []candidate;
		}
	}while (count < max_no_improv);
	return best;
}

int* search(int* cities, int neigh, int max_no_improv, int max_no_improv_ls){
	int iter, count;
	int* best = random_permutation(cities);
	best[CITIES] = cost(best, cities);
	iter = count = 0;
	do{
		for(int i=0; i<neigh; ++i){
			int* candidate = new int[CITIES+1];
			copy(&best[0], &best[CITIES+1], &candidate[0]);
			for(int j=0; j<i; ++j) stochastic_two_opt(candidate);
			candidate[CITIES] = cost(candidate, cities);
			candidate = local_search(candidate, cities, max_no_improv_ls, i);
			cout<<" > iteration "<<iter+1<<", neigh="<<i<<", best="<<best[CITIES]<<endl;
			++iter;
			if(candidate[CITIES] < best[CITIES]){
				count = 0;
				delete []best;
				best = candidate;
				cout<<"New best, restarting neighborhood search."<<endl;
			} else {
				++count;
				delete []candidate;
			}
		}
	}while (count < max_no_improv);
	return best;
}

int main(){
	int berlin[52][2] = {{565,575},{25,185},{345,750},{945,685},{845,655},
		{880,660},{25,230},{525,1000},{580,1175},{650,1130},{1605,620},
		{1220,580},{1465,200},{1530,5},{845,680},{725,370},{145,665},
		{415,635},{510,875},{560,365},{300,465},{520,585},{480,415},
		{835,625},{975,580},{1215,245},{1320,315},{1250,400},{660,180},
		{410,250},{420,555},{575,665},{1150,1160},{700,580},{685,595},
		{685,610},{770,610},{795,645},{720,635},{760,650},{475,960},
		{95,260},{875,920},{700,500},{555,815},{830,485},{1170,65},
		{830,610},{605,625},{595,360},{1340,725},{1740,245}};
	int max_no_improv = 500;
	int max_no_improv_ls = 120;
	int neighborhoods = 40;
	int* best = search(&berlin[0][0], neighborhoods, max_no_improv, max_no_improv_ls);
	cout<<"Done. Best Solution: c="<<best[CITIES]<<", v=";
	for(int i=0; i<CITIES-1; ++i)
		cout<<best[i]<<", ";
	cout<<best[CITIES-1];
	return 0;
}