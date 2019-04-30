#include<iostream>
#include <cmath>
#include <limits>
#include "Line.h"
#include "Vec.h"

using namespace std;

  Line::Line () : Line(0, 0, 0, 0) { }
  Line::Line (Vec start, Vec end) : Line(start.getX(), start.getY(), end.getX(), end.getY()) { }
  Line::Line(double x1, double y1, double x2, double y2) {
    this->setStart(x1, y1);
    this->setEnd(x2, y2);
    this->setMagnitude(start, end);
  }
  Line::~Line () {};

  void Line::setStart(double x, double y) {
    this->start = Vec(x, y);
  }

  void Line::setEnd(double x, double y) {
    this->end = Vec(x, y);
  }

  void Line::setMagnitude(double x, double y) {
    this->magnitude = Vec(x, y);
  }

  void Line::setMagnitude(Vec &start, Vec &end) {
    this->magnitude = end - start;
  }

  Vec Line::getStart() const{
    return this->start;
  }

  Vec Line::getEnd() const{
    return this->end;
  }

  Vec Line::getMagnitude() const{
    return this->magnitude;
  }

  bool Line::coincident(Line const &other) {
    //cout << other << "\n";
    return Vec::cross(other.getStart() - start, magnitude) == 0 && Vec::cross(other.getEnd() - start, magnitude) == 0;
  }

  Vec Line::intersection(Line other) const{
    Vec l1 = start;
    Vec mag1 = magnitude;
    Vec l2 = (other.getStart());
    Vec mag2 = (other.getMagnitude());
    double inf = std::numeric_limits<double>::infinity();

    //cout << "Parallel?: " << Vec::cross(mag1, mag2) << "\n";
    if(Vec::cross(mag1, mag2) == 0) {
      return Vec(inf, inf);
    } else {
      double s = (mag2.getX() * (l2.getY() - l1.getY())) - ((l2.getX() - l1.getX()) * mag2.getY());
      s = s / ((mag2.getX() * mag1.getY()) - (mag1.getX() * mag2.getY()));

      double t = (mag1.getX() * (l2.getY() - l1.getY())) - ((l2.getX() - l1.getX()) * mag1.getY());
      t = t / ((mag2.getX() * mag1.getY()) - (mag1.getX() * mag2.getY()));

      //cout << "s = " << s <<"\n";
      //cout << "t = " << t <<"\n";

      if ((s >= 0 && s <= 1) && (t >= 0 && t <= 1)) {
        return start + (magnitude * s);
      } else {
        return Vec(inf, inf);
      }
    }
  }

  Vec Line::intersectionINF(Line other) const{
    //cout << "Line to be checked: " << other << "\n";
    Vec l1 = start;
    Vec mag1 = magnitude;
    Vec l2 = (other.getStart());
    Vec mag2 = (other.getMagnitude());
    double inf = std::numeric_limits<double>::infinity();

    //cout << "Mag1: " << mag1 << "\n";
    //cout << "Mag2: " << mag2 << "\n";
    //cout << "Parallel?: " << Vec::cross(mag1, mag2) << "\n";
    if(Vec::cross(mag1, mag2) == 0) {
      return Vec(inf, inf);
    } else {
      double s = (mag2.getX() * (l2.getY() - l1.getY())) - ((l2.getX() - l1.getX()) * mag2.getY());
      s = s / ((mag2.getX() * mag1.getY()) - (mag1.getX() * mag2.getY()));

      double t = (mag1.getX() * (l2.getY() - l1.getY())) - ((l2.getX() - l1.getX()) * mag1.getY());
      t = t / ((mag2.getX() * mag1.getY()) - (mag1.getX() * mag2.getY()));

      //cout << "s = " << s <<"\n";
      //cout << "t = " << t <<"\n";
      //cout << start << " + " << magnitude << " * " << s << " : " << (start + (magnitude * s)) << "\n";
      return start + (magnitude * s);
    }
  }

  bool Line::operator == (Line const &line) {
    return this->getStart() == line.getStart() && this->getMagnitude() == line.getMagnitude();
  }

  ostream& operator<< (ostream& os, Line const &line) {
    os << line.getStart() << " + t" << line.getMagnitude();
    return os;
  }


  // int main() {
  //   Line l1(0, 0, -1, 1);
  //   cout << "Line 1: " << l1 << "\n";
  //   Vec n(-1,0);
  //   cout << "Point: " << n << "\n";
  //   double a = Vec::cross(n - l1.getStart(), l1.getMagnitude());
  //   cout << a << "\n";
  // }
