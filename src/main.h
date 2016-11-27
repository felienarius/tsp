#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>
#include "Node.cpp"
#include "Arc.cpp"
#include "Graph.cpp"
#include "Config.cpp"
#include "Route.cpp"
#include "BranchAndBound.cpp"
#include "GraphTransformation.cpp"

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;

void TestNode();
void TestArc();
void TestGraph();
void runTests();
void doGraphTransformation(unsigned int index = 100);
