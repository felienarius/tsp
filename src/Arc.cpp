class Arc {
 private:
  const Node *start;
  const Node *end;
  unsigned int distance;
  unsigned int time;
 public:
  Arc(const Node &start, const Node &end, const unsigned int distance = 0,
      const unsigned int time = 0);
  bool operator<(const Arc &a) const;
  unsigned int getStart() const;
  unsigned int getEnd() const;
  unsigned int getDistance() const;
  unsigned int getTime() const;
  unsigned int getTimeInstance() const;
  std::string toString() const;
  void print() const;
};

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