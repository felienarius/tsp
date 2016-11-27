#ifndef ARC_H
#define ARC_H
#include "Node.h"

class Arc {
 private:
  const Node *start;
  const Node *end;
  unsigned int distance;
  unsigned int time;
 public:
  Arc(const Node &start, const Node &end,
    const unsigned int distance = 0,
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

#endif