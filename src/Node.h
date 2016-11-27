#ifndef NODE_H
#define NODE_H

class Node {
 private:
  unsigned int index;
  unsigned int timeInstance;
  unsigned int open;
  unsigned int close;
 public:
  Node(const unsigned int index, const unsigned int timeInstance = 0,
       const unsigned int open = 0, const unsigned int close = 0);
  bool operator<(const Node &n) const;
  bool operator==(const Node &n) const;
  unsigned int getIndex() const;
  unsigned int getOpen() const;
  unsigned int getClose() const;
  unsigned int getTimeInstance() const;
  std::string toString() const;  
  void print() const;
};
#endif