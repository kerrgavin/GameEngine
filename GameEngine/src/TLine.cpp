#include "Vec.h"
#include "Line.h"
#include "TLine.h"

using namespace std;

TLine::TLine() : TLine(0, 0, 0, 0, "", "", "") {}
TLine::TLine (Vec start, Vec end) : TLine(start.getX(), start.getY(), end.getX(), end.getY(), "", "", "") { }
TLine::TLine(double x1, double y1, double x2, double y2) : TLine(x1, y1, x2, y2, "", "", "") { }
TLine::TLine(double x1, double y1, double x2, double y2, string middle, string top, string bottom) : Line(x1, y1, x2, y2) {
  setMiddle(middle);
  setTop(top);
  setBottom(bottom);
}
TLine::~TLine () {}

void TLine::setMiddle(string middle) {
  this->middle = middle;
}
void TLine::setTop(string top) {
  this->top = top;
}
void TLine::setBottom(string bottom) {
  this->bottom = bottom;
}

string TLine::getMiddle() const {
  return this->middle;
}
string TLine::getTop() const {
  return this->top;
}
string TLine::getBottom() const {
  return this->bottom;
}
