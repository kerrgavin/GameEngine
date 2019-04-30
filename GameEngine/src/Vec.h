#ifndef CUSTOMVEC_H
#define CUSTOMVEC_H

#include<iostream>

using namespace std;

class Vec {

private:
  double x, y;

public:
  Vec ();
  Vec (double x, double y);
  virtual ~Vec ();

  void setX(double x);
  void setY(double y);
  double getX() const;
  double getY() const;
  double getProduct();
  double getDoubleMag() const;

  Vec operator + (Vec const &vec) const;
  Vec operator - (Vec const &vec);
  Vec operator + (double const &scalar) const;
  Vec operator - (double const &scalar);
  Vec operator * (double const &scalar) const;
  Vec operator * (int const &scalar) const;
  //double operator * (Vec const &vec);
  Vec operator / (double const &scalar);
  Vec operator / (int const &scalar);
  bool operator == (Vec const &vec);
  //ostream& operator<< (ostream& os, Vec const &vec);
  //static Vec crossProduct(Vec const &vec1, Vec const &vec2);
  double static angle (Vec const &vec1, Vec const &vec2);
  double static cross (Vec const &vec1, Vec const &vec2);

};
ostream& operator<< (ostream& os, Vec const &vec);
double operator * (Vec const &vec1, Vec const &vec2);
#endif
