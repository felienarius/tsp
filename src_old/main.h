// #include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
// #include "vns.cpp"
#include "BranchAndBound.cpp"
#include "Route.cpp"

// TODO koszt przejazdu zależny od czasu
// okna czasowe nie są złośliwe
// przyjąć pracę od 7 do 19/17

const int MAX_DELAY = 0;

const int GODZINY_PRACY = 10;
const double KOSZT_GODZINA = 15;
const double NADGODZINA_KOSZT = 30;
const double DROGA_KOSZT = 0.4;
const double KOSZT_SPOZNIENIA = 2.0;
const double KOSZT_NIEDOSTARCZENIA = 30;

Graph *firstAuxiliaryGraph(Data *d);

int loadATSP(string name, int ***dist);
int **calcTimeTravel(int n, int **dist, int max_t);
int **generateTimeWindows(int n, int close, int maxService, int seed = 5);
int *generateHourMultipler(int n, int variation = 15, int seed = 5);
void printTab(int n, int m, int **t);
void printTab(int n, int *t);
void printData(Data *d);
void printRoute(Route *r);

Route *search(Data *d, int neighborhoods, int max_no_improv, int max_no_improv_ls);
Route *firstRoute(Data *d);
int *sortTimeWindows(int n, int **win);
int calcRouteTravelTimeEndingAt(Data *d, Route *r, int index, int node);
int calcRouteTravelTimeStartingAt(Data *d, Route *r, int index , int node , int k);
void insertNode(Route *r, int index, int node);

void calcRoute(Data *d, Route *r);
long calcRouteCost(Route *r);
void stochastic_two_opt(Route *r);
void local_search(Data *d, Route *best, int max_no_improv, int nhood);
void calcBestStartTime(Data *d, Route *route);
