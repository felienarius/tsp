#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<cmath>

using namespace std;

const static int CITIES = 5;

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
	return sqrt((c1[0] - c2[0])^2 + (c1[1] - c2[1])^2);
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
	int* perm = new int[CITIES];
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

void local_search(int* best, int* bestcost, int* cities, int max_no_improv, neighborhood){
	int count = 0;
	do{
		
	}while (count < max_no_improv)
}

int main(){
	int a[] = {5,22,33,44,57};
	reverse(&a[0], 57,22);
	cout<<	a[0]<<
	a[1]<<
	a[2]<<
	a[3]<<
	a[4]<<endl;
	return 0;
}