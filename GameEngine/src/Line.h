#ifndef LINE_H
#define LINE_H
#include "Vec.h"

using namespace std;

class Line {

private:
  Vec start, end, magnitude;

public:
  Line();
  Line(Vec start, Vec end);
  Line (double x1, double y1, double x2, double y2);
  virtual ~Line ();

  void setStart(double x, double y);

  void setEnd(double x, double y);

  void setMagnitude(double x, double y);
  void setMagnitude(Vec &start, Vec &end);

  Vec getStart() const;

  Vec getEnd() const;

  Vec getMagnitude() const;

  bool operator == (Line const &line);

  bool coincident(Line const &other);

  Vec intersection(Line other) const;
  Vec intersectionINF(Line other) const;
};
ostream& operator<< (ostream& os, Line const &line);

#endif
