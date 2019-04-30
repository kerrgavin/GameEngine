#ifndef CHARACTER_H
#define CHARACTER_H

#include<vector>
#include "Line.h"
#include "Vec.h"
#include "Poly.h"

using namespace std;

class Character {

private:
  double height;
  double z;
  Vec position;
  double angleX;
  double angleY;
  double angleZ;
  double r;
  double s;
  Vec rightFOV;
  Vec leftFOV;

public:
  Character ();
  Character (double x, double y, double z, double height, double angleX, double angleZ);
  virtual ~Character ();

  void setPosition(double x, double y, double z);
  void setDirection(double angleX, double angleZ);
  void setHeight(double height);
  Vec getPosition() const;
  double getZ() const;
  Vec getRightFOV() const;
  Vec getLeftFOV() const;
  double getAngleX() const;
  double getAngleY() const;
  double getAngleZ() const;
  double getHeight() const;
  bool inView(TLine line);
  bool inView(Vec v);
  Vec pointOnScreen(Vec point, double z);
  vector<double> genWall(TLine const &line, double floorHeight, double ceilingHeight);
  vector<double> genPoly(Poly const &Poly, double z);
};

#endif
