#ifndef CUSTOMPOLY_H
#define CUSTOMPOLY_H

#include<iostream>
#include<memory>
#include "TLine.h"

using namespace std;

class Poly {

private:
  vector<TLine> lines;
  vector<Vec> vertices;
  double ceilingHeight, floorHeight;
  string ceiling, floor;

public:
  Poly ();
  virtual ~Poly ();

  void setCeilingHeight(double ceilingHeight);
  void setFloorHeight(double floorHeight);
  void setCeiling(string ceiling);
  void setFloor(string floor);
  void addLine(TLine line);
  vector<TLine> getLines() const;
  double getCeilingHeight() const;
  double getFloorHeight() const;
  string getCeiling() const;
  string getFloor() const;
  vector<Vec> getVertices() const;
  void genVertices();
  int static side (Poly const &poly, TLine const &line);
  vector<Poly> static subSections(Poly const &poly, TLine const &line);
};

ostream& operator<< (ostream& os, Poly const &poly);

#endif
