#ifndef TLINE_H
#define TLINE_H
#include "Vec.h"
#include "Line.h"

using namespace std;

class TLine : public Line {

private:
  string middle, top, bottom;

public:
  TLine();
  TLine(double x1, double y1, double x2, double y2);
  TLine (Vec start, Vec end);
  TLine(double x1, double y1, double x2, double y2, string middle, string top, string bottom);
  virtual ~TLine ();

  void setMiddle(string middle);
  void setTop(string top);
  void setBottom(string bottom);

  string getMiddle() const;
  string getTop() const;
  string getBottom() const;
};

#endif
