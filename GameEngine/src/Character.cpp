#define _USE_MATH_DEFINES
#include<cmath>
#include<vector>
#include "Vec.h"
#include "Line.h"
#include "TLine.h"
#include "Character.h"
#include "Poly.h"

  Character::Character () : Character(0, 0, 0, 1, M_PI_4, 0) { }
  Character::Character (double x, double y, double z, double height, double angleX, double angleZ) {
    setPosition(x, y , z);
    setHeight(height);
    this->angleY = M_PI;
    r = 1;
    s = 1;
    setDirection(angleX, angleZ);
  }
  Character::~Character () { }

  void Character::setPosition(double x, double y, double z) {
    this->position = Vec(x, y);
    this->z = z;
    //cout << position << "\n";
  }
  void Character::setDirection(double angleX, double angleZ){
    this->angleX = angleX;
    this->angleZ = angleZ;
    this->leftFOV = Vec(cos(angleZ + M_PI_4), sin(angleZ + M_PI_4));
    //cout << "LeftFOV: " << leftFOV << "\n";
    this->rightFOV = Vec(cos(angleZ - M_PI_4), sin(angleZ - M_PI_4));
    //cout << "RightFOV: " << rightFOV << "\n";
  }
  Vec Character::getPosition() const {
    return this->position;
  }

  double Character::getZ() const {
    return this->z;
  }

  Vec Character::getRightFOV() const {
    return this->rightFOV;
  }
  Vec Character::getLeftFOV() const {
    return this->leftFOV;
  }
  double Character::getAngleX() const {
    return this->angleX;
  }
  double Character::getAngleY() const {
    return this->angleY;
  }

  double Character::getAngleZ() const {
    return this->angleZ;
  }

  void Character::setHeight(double height) {
    this->height = height;
  }

  double Character::getHeight() const {
    return this->height;
  }

  bool Character::inView(TLine line) {
    Vec inter = line.intersection(Line(position, rightFOV));
    if(!isinf(inter.getX())) {
      return true;
    }
    inter = line.intersection(Line(position, leftFOV));
    if(!isinf(inter.getX())) {
      return true;
    }
    return inView(line.getStart()) && inView(line.getEnd());
  }

  bool Character::inView(Vec v) {
    double a = Vec::cross(v - position, rightFOV);
    double b = Vec::cross(v - position, leftFOV);
    //cout << "Point: " << v << "\n";
    //cout << "A: " << a << "\n";
    //cout << "B: " << b << "\n";
    if (angleZ >= M_PI_4 && angleZ < 3 * M_PI_4) {
      return a/abs(a) == 1 && b/abs(b) == 1;
    }
    else if (angleZ >= 3 * M_PI_4 && angleZ < 5 * M_PI_4) {
      return a/abs(a) == -1 && b/abs(b) == 1;
    }
    else if (angleZ >= 5 * M_PI_4 && angleZ < 7 * M_PI_4) {
      return a/abs(a) == -1 && b/abs(b) == -1;
    }
    else {
      return a/abs(a) == 1 && b/abs(b) == -1;
    }
  }

  Vec Character::pointOnScreen(Vec point, double z) {
    //cout << "Start" << "\n";
    //cout << "Angle x: " << angleX << "\n";
    //cout << "Angle y: " << angleY << "\n";
    //cout << "Angle z: " << angleZ << "\n";
    double dX = point.getX() - position.getX();
    double dY = point.getY() - position.getY();
    double dZ = z - this->z;

    //cout << dX << "\n";
    //cout << dY << "\n";
    //cout << dZ << "\n";

    double tX = cos(angleY) * (sin(angleZ) * dY + cos(angleZ) * dX) - sin(angleY) * dZ;
    double tY = sin(angleX) * ((cos(angleY) * dZ) + (sin(angleY) * ((sin(angleZ) * dY) + (cos(angleZ) * dX)))) + (cos(angleX) * ((cos(angleZ) * dY) - (sin(angleZ) * dX)));
    double tZ = cos(angleX) * (cos(angleY) * dZ + sin(angleY) * (sin(angleZ) * dY + cos(angleZ) * dX)) - sin(angleX) * (cos(angleZ) * dY - sin(angleZ) * dX);

    //cout << "Y part one: " << sin(angleX) * ((cos(angleY) * dZ) + (sin(angleY) * ((sin(angleZ) * dY) + (cos(angleZ) * dX)))) << "\n";
    //cout << "Y part two: " << (cos(angleX) * ((cos(angleZ) * dY) - (sin(angleZ) * dX))) << "\n";

    //cout << tX << "\n";
    //cout << tY << "\n";
    //cout << tZ << "\n";

    double bX = (tX * s) / ((tZ / r) * r);
    double bY = (tY * s) / ((tZ / r) * r);

    return Vec(bX, bY);
  }

  vector<double> Character::genWall(TLine const &line, double floorHeight, double ceilingHeight) {
    Vec point1 = pointOnScreen(line.getStart(), floorHeight);
    Vec point2 = pointOnScreen(line.getStart(), ceilingHeight);
    Vec point3 = pointOnScreen(line.getEnd(), ceilingHeight);
	Vec point4 = pointOnScreen(line.getEnd(), floorHeight);

    vector<double> points;
    points.push_back(point1.getX());
    points.push_back(point1.getY());
    points.push_back(point2.getX());
    points.push_back(point2.getY());
    points.push_back(point3.getX());
    points.push_back(point3.getY());
    points.push_back(point4.getX());
    points.push_back(point4.getY());

    return points;
  }

  vector<double> Character::genPoly(Poly const &poly, double z) {
    vector<double> points;
    for (auto const &i : poly.getVertices()) {
      Vec point = pointOnScreen(i, z);
	  //cout << "Poly point: " << point << endl;
      points.push_back(point.getX());
      points.push_back(point.getY());
    }
    return points;
  }

 /* int main() {
    Character c;
    c.setHeight(1);
    c.setPosition(0, 0, 1);
    c.setDirection(M_PI_2, 0);
    TLine l(-0.5, 1, 0.5 , 1);
    vector<double> v= c.genWall(l, 1, 2);
    cout << "Out" << "\n";
    for (auto const &i : v) {
      cout << i << "\n";
    }
  }*/
