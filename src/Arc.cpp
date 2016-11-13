class Arc {
private:
  unsigned int start;
  unsigned int end;
  unsigned int value;
public:
  Arc(unsigned int _start, unsigned int _end, unsigned int _value);
  bool operator< (const Arc& a) const;
  unsigned int getValue() const;
  unsigned int getStart() const;
  unsigned int getEnd() const;
  void setValue(unsigned int v);
};

Arc::Arc(unsigned int _start, unsigned int _end, unsigned int _value = 0) {
  start = _start ;
  end = _end;
  value = _value;
}

bool Arc::operator< (const Arc& a) const {
  return (start < a.start) || (start == a.start && end < a.end);
}

unsigned int Arc::getStart() const {
  return start;
}

unsigned int Arc::getEnd() const {
  return end;
}

unsigned int Arc::getValue() const {
  return value;
}

void Arc::setValue(unsigned int v) {
  value = v;
}