#include "Arc.h"

Arc::Arc(const Node &start, const Node &end, const unsigned int distance,
         const unsigned int time) {
  this->start = &start;
  this->end = &end;
  this->distance = distance;
  this->time = time;
}

bool Arc::operator<(const Arc &a) const {
  return (*start < *a.start || *start == *a.start && *end < *a.end);
}

unsigned int Arc::getStart() const {
  return start->getIndex();
}

unsigned int Arc::getEnd() const {
  return end->getIndex();
}

unsigned int Arc::getDistance() const {
  return distance;
}

unsigned int Arc::getTime() const {
  return time;
}

unsigned int Arc::getTimeInstance() const {
  return start->getTimeInstance();
}

std::string Arc::toString() const {
  std::stringstream ss;
  ss << "A{" << start->toString() << " => " << end->toString() << ";  ["
     << getDistance() << ", " << getTime() << "]}";
}

void Arc::print() const {
  std::cout << toString() << std::endl;
}