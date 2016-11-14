class Arc {
private:
  unsigned int start;
  unsigned int end;
  unsigned int distance;
  unsigned int time;
  unsigned int timeInstance;
public:
  Arc(unsigned int start, unsigned int end);
  Arc(unsigned int start, unsigned int end, unsigned int distance, unsigned int time, unsigned int  timeInstance);
  bool operator< (const Arc& a) const;
  unsigned int getStart() const;
  unsigned int getEnd() const;
  unsigned int getDistance() const;
  unsigned int getTime() const;
  unsigned int getTimeInstance() const;
  void setDistance(unsigned int distance);
};

Arc::Arc(unsigned int start, unsigned int end) {
  this->start = start ;
  this->end = end;
  distance = 0;
  time = 0;
  timeInstance = 0;
}

Arc::Arc(unsigned int start, unsigned int end, unsigned int distance, unsigned int time, unsigned int  timeInstance) {
  this->start = start ;
  this->end = end;
  this->distance = distance;
  this->time = time;
  this->timeInstance = timeInstance;
}

bool Arc::operator< (const Arc& a) const {
  return (start < a.start) || (start == a.start && end < a.end) || (start == a.start && end == a.end && timeInstance < a.timeInstance);
}

unsigned int Arc::getStart() const {
  return start;
}

unsigned int Arc::getEnd() const {
  return end;
}

unsigned int Arc::getDistance() const {
  return distance;
}
unsigned int Arc::getTime() const {
  return time;
}
unsigned int Arc::getTimeInstance() const {
  return timeInstance;
}

void Arc::setDistance(unsigned int distance) {
  this->distance = distance;
}