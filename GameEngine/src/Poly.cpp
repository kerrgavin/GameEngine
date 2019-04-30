#include<iostream>
#include<cmath>
#include<memory>
#include<limits>
#include<vector>
#include "Poly.h"
#include "Line.h"
#include "Vec.h"

using namespace std;

  Poly::Poly() {};
  Poly::~Poly () {};

  void Poly::setCeilingHeight(double ceilingHeight) {
    this->ceilingHeight = ceilingHeight;
  }
  void Poly::setFloorHeight(double floorHeight){
    this->floorHeight = floorHeight;
  }
  void Poly::setCeiling(string ceiling) {
    this->ceiling = ceiling;
  }
  void Poly::setFloor(string floor) {
    this->floor = floor;
  }

  double Poly::getCeilingHeight() const {
    return this->ceilingHeight;
  }
  double Poly::getFloorHeight() const {
    return this->floorHeight;
  }
  string Poly::getCeiling() const {
    return this->ceiling;
  }
  string Poly::getFloor() const {
    return this->floor;
  }

  void Poly::addLine(TLine line) {
    this->lines.push_back(line);
  }

  vector<TLine> Poly::getLines() const{
    return this->lines;
  }

  vector<Poly> Poly::subSections(Poly const &poly, TLine const &line) {
    //cout << "Getting Subsections" << "\n";
    double inf = std::numeric_limits<double>::infinity();
    Vec point1(inf,inf);
    Vec point2(inf, inf);
    Line l1;
    Line l2;
    Poly poly1;
    Poly poly2;
    vector<Line> lineSet;
    vector<Poly> r;

    Line polyLine;
    for(int i = 0; i < poly.getLines().size(); i++) {
      polyLine = poly.getLines()[i];
      //cout << "Checking line: " << polyLine << "\n";
      Vec temp = line.intersection(polyLine);

      if (!isinf(temp.getX())) {
        //cout << "There is an intersection" << "\n";
        if (isinf(point1.getX())) {
          //cout << "First not set" << "\n";
          point1 = Vec(temp.getX(), temp.getY());
          l1 = polyLine;
        } else {
          if (!(temp == point1)) {
            //cout << "intersection not point1" << "\n";
            if (isinf(point2.getX())) {
              point2 = Vec(temp.getX(), temp.getY());
              l2 = polyLine;
            } else {
              lineSet.push_back(polyLine);
            }
          } else {
            //cout << "Doesn't intersect, adding line: " << polyLine << "\n";
            lineSet.push_back(polyLine);
          }
        }
      } else {
        //cout << "Doesn't intersect, adding line: " << polyLine << "\n";
        lineSet.push_back(polyLine);
      }
    }

    //cout << "Point 1: " << point1 << "\n";
    //cout << "Point 2: " << point2 << "\n";
    if (isinf(point1.getX()) || isinf(point1.getY()) || isinf(point2.getX()) || isinf(point2.getY())) {
      return r;
    }

    Line test = Line(point1, point2);
    for (auto const &i : poly.getLines()) {
      if (test.coincident(i)) {
        return r;
      }
    }


    // cout << "Point1: " << point1 << "\n";
    // cout << "Point2: " << point2 << "\n";
    // cout << "Line1: " << l1 << "\n";
    // cout << "Line2: " << l2 << "\n";

    //if (isinf(point1.getX()) && isinf(point1.getY())) {
    //  r.push_back(poly);
    //} else {
    if (!(isinf(point1.getX()) && isinf(point1.getY()))) {
      if(point1 == l1.getStart() || point1 == l1.getEnd()) {
        lineSet.push_back(l1);
      } else {
        lineSet.push_back(TLine(point1, l1.getStart()));
        lineSet.push_back(TLine(point1, l1.getEnd()));
      }
      if (point2 == l2.getStart() || point2 == l2.getEnd()) {
        lineSet.push_back(l2);
      } else {
        lineSet.push_back(TLine(point2, l2.getStart()));
        lineSet.push_back(TLine(point2, l2.getEnd()));
      }

      //cout << "Full Line Set" << "\n";
      //for (auto const &i : lineSet) {
        //cout << i << '\n';
      //}

      for (auto const &s : lineSet) {
        Vec start = s.getStart();
        Vec end = s.getEnd();
        //cout << "For Line: " << s << " it is located on side: " << Vec::cross(line.getMagnitude(), start - line.getStart()) << " "  << Vec::cross(line.getMagnitude(), end - line.getStart()) << "\n";
        if(Vec::cross(start - line.getStart(), line.getMagnitude()) > 0 || Vec::cross(end - line.getStart(), line.getMagnitude()) > 0) {
          poly1.addLine(TLine(s.getStart().getX(), s.getStart().getY(), s.getEnd().getX(), s.getEnd().getY()));
        } else {
          poly2.addLine(TLine(s.getStart().getX(), s.getStart().getY(), s.getEnd().getX(), s.getEnd().getY()));
        }
      }

      poly1.addLine(TLine(point1.getX(), point1.getY(), point2.getX(), point2.getY()));
      poly2.addLine(TLine(point1.getX(), point1.getY(), point2.getX(), point2.getY()));

      r.push_back(poly1);
      r.push_back(poly2);
    }
    return r;
  }

  int Poly::side (Poly const &poly, TLine const &line) {
    for (auto const &s : poly.getLines()) {
      Vec start = s.getStart();
      Vec end = s.getEnd();
      double startCross = Vec::cross(start - line.getStart(), line.getMagnitude());
      double endCross = Vec::cross(end - line.getStart(), line.getMagnitude());
      //cout << s << "\n";
      //cout << line << "\n";
      //cout << start << " " << end << "\n";
      //cout << startCross << " " << endCross << "\n";
      if(startCross > 0 || endCross > 0) {
        return 1;
      } else if (startCross < 0 || endCross < 0) {
        return -1;
      }
    }
    return 0;
  }

  void Poly::genVertices() {
    for (auto const &line : this->getLines()) {
      bool startIn = false;
      bool endIn = false;
      for (auto const &point : vertices) {
        if (line.getStart() == point) {
          startIn = true;
        }
        if (line.getEnd() == point) {
          endIn = true;
        }
      }
      if (!startIn) {
        vertices.push_back(line.getStart());
      }
      if(!endIn) {
        vertices.push_back(line.getEnd());
      }
    }
  }


  vector<Vec> Poly::getVertices() const{
    return this->vertices;
  }

  ostream& operator<< (ostream& os, Poly const &poly) {
    for (auto const &i : poly.getLines()) {
      os << i << '\n';
    }
    return os;
  }


  // int main() {
  //   TLine l1(0, 0, 0, 1);
  //   TLine l2(0, 1, 1, 1);
  //   TLine l3(1, 1, 1, 0);
  //   TLine l4(1, 0, 0, 0);
  //
  //   TLine l5(1, 0, 0, 0);
  //   Poly poly;
  //   poly.addLine(l1);
  //   poly.addLine(l2);
  //   poly.addLine(l3);
  //   poly.addLine(l4);
  //   cout << "Poly" << "\n";
  //   cout << poly << "\n";
  //   cout << "Line: " << l5 << "\n";
  //   vector<Poly> v = Poly::subSections(poly, l5);
  //   cout << v.size() << "\n";
  //   cout << Poly::side(poly, l5) << "\n";
  //
  //   poly.genVertices();
  //   cout << poly.getVertices().size() << "\n";
  // }
