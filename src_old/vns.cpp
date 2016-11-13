#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include <iostream>

using namespace std;

void printT(int n, int *t){
	int i;
	cout << "[";
	for (i = 0; i < n - 1 ; ++i) cout << t[i] << ", ";
	cout << t[i] << "]\n";
}



// int cost(int n, int **dist, int **t, int **w, int *f, int *perm, int *last_valid) {
// 	cout << ">>cost " << perm << /*" " << last_valid << */endl;
// 	int distance, delay, missed, close, k, i, j, a, cost;
// 	// printT(20, perm);

// 	distance = delay = missed = 0;
// 	k = perm[0];

// 	j = perm[1];
// 	for (a = 1; a < n + 1; ++a) {
// 		i = j;
// 		j = perm[a + 1];
// 		k += w[i][2];
// 		k += max(t[i][j] * f[k/60]/1000, w[j][0]);
// 		distance += dist[i][j];
// 		close = w[j][1];
// 		if (k > close) {
// 			if (k > close + MAX_DELAY) ++missed;
// 			delay += k - close;
// 		}
// 	}
// 	// if (missed == 0) {
// 	// 	delete []last_valid;
// 	// 	copy(&perm[0], &perm[n + 3], &last_valid[0]);
// 	// }
// 	cost = f_cost(distance, k, missed, delay);
// 	cout << "<<cost " << perm << " " /*<< last_valid << " "*/ << cost << endl;
// 	return cost;
// }












void reverse(int *perm, int i1, int i2){
	cout << ">>reverse " << perm << " " << i1 << " " << i2 <<endl;
	printT(20, perm);
	int a, b;
	a = i1;
	b = i2;
	if (a > b) {
		for (;a > b; --a, ++b)
			swap(perm[a], perm[b]);
	} else { /* a < b */
		for (;a < b; ++a, --b)
			swap(perm[a], perm[b]);
	}
	printT(20, perm);
	cout << "<<reverse " << perm << " " << i1 << " " << i2 <<endl;
}

void stochastic_two_opt(int n, int* perm){
	cout << ">>stochastic_opt-2 " << n << " " << perm <<"\n";
	int index1, index2;
	int *e = new int[3];
	index1 = rand() % n;
	index2 = rand() % n;
	e[0] = index1;
	e[1] = (index1 + 1) % n;
	e[2] = (index1 + n - 1) % n;
	while (index2 == e[0] || index2 == e[1] || index2 == e[2])
		index2 = rand() % n;
	delete []e;
	/* reverse [i1+2, i2+2] */
	reverse(perm, index1+2, index2+2);
	cout << "<<stochastic opt-2 " << n << " " << perm <<"\n";
}

// int* local_search(int n, int *best, int **dist, int **t, int **win, int *f,
// int *last_valid, int max_no_improv, int nhood)
// {
// 	cout << ">>local_search " << best << " " << nhood << "\n";
// 	int i, count = 0;
// 	do {
// 		int* candidate = new int[n + 3];
// 		copy(&best[0], &best[n + 3], &candidate[0]);
// 		for (i = 0; i < nhood; ++i)
// 			stochastic_two_opt(n, candidate);
// 		candidate[0] = cost(n, dist, t, win, f, candidate, last_valid);
// 		printT(20, candidate);
// 		if (candidate[0] < best[0]) {
// 			count = 0;
// 			delete []best;
// 			best = candidate;
// 		} else {
// 			++count;
// 			delete []candidate;
// 		}
// 	} while (count < max_no_improv);
// 	cout << "<<local_search " << best << " " << nhood << "\n";
// 	return best;
// }



// int main2(){
// 	int berlin[52][2] = {{565,575},{25,185},{345,750},{945,685},{845,655},
// 		{880,660},{2,230},{525,1000},{580,1175},{650,1130},{1605,620},
// 		{1220,580},{1465,200},{1530,5},{845,680},{725,370},{145,665},
// 		{415,635},{510,875},{560,365},{300,465},{520,585},{480,415},
// 		{835,625},{975,580},{1215,245},{1320,315},{1250,400},{660,180},
// 		{410,250},{420,555},{575,665},{1150,1160},{700,580},{685,595},
// 		{685,610},{770,610},{795,645},{720,635},{760,650},{475,960},
// 		{95,260},{875,920},{700,500},{555,815},{830,485},{1170,65},
// 		{830,610},{605,625},{595,360},{1340,725},{1740,245}};
// 	int max_no_improv = 500;
// 	int max_no_improv_ls = 120;
// 	int neighborhoods = 40;
// 	int* best = search(&berlin[0][0], neighborhoods, max_no_improv, max_no_improv_ls);
// 	cout<<"Done. Best Solution: c="<<best[CITIES]<<", v=";
// 	for(int i=0; i<CITIES-1; ++i)
// 		cout<<best[i]<<", ";
// 	cout<<best[CITIES-1];
// 	return 0;
// }