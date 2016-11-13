#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "Config.cpp"

using std::ifstream;
using std::string;
using std::vector;
using std::max;

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;

class GraphTransformation {
private:
  shared_ptr<Config> config;
  unique_ptr<Graph> graph;
public:
  // GraphTransformation();
  void load(shared_ptr<Config> config);
};

void GraphTransformation::load(shared_ptr<Config> config) {
  this->config = config;
  this->config->print();
}
