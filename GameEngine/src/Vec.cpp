#include <cmath>
#include "Vec.h"
#include<iostream>

using namespace std;
Vec::Vec () : Vec(0, 0) { }
Vec::Vec(double x, double y) {
  this->setX(x);
  this->setY(y);
}

Vec::~Vec() {};

void Vec::setX(double x) {
  this->x = x;
}

void Vec::setY(double y) {
  this->y = y;
}

double Vec::getX() const{
  return this->x;
}

double Vec::getY() const{
  return this->y;
}

double Vec::getProduct() {
  return this->x * this->y;
}

double Vec::getDoubleMag() const{
  return pow(this->x,2) + pow(this->y, 2);
}

Vec Vec::operator + (Vec const &vec) const{
  Vec newVec(this->getX() + vec.getX(), this->getY() + vec.getY());
  return newVec;
}

Vec Vec::operator + (double const &scalar) const{
  Vec newVec(this->getX() + scalar, this->getY() + scalar);
  return newVec;
}

Vec Vec::operator - (Vec const &vec) {
  Vec newVec(this->getX() - vec.getX(), this->getY() - vec.getY());
  return newVec;
}

Vec Vec::operator - (double const &scalar) {
  Vec newVec(this->getX() - scalar, this->getY() - scalar);
  return newVec;
}

Vec Vec::operator * (double const &scalar) const {
  Vec newVec(this->getX() * scalar, this->getY() * scalar);
  return newVec;
}

Vec Vec::operator * (int const &scalar) const {
  Vec newVec(this->getX() * scalar, this->getY() * scalar);
  return newVec;
}

double operator * (Vec const &vec1, Vec const &vec2) {
    return vec1.getX() * vec2.getX() + vec1.getY() * vec2.getY();
}

double Vec::angle (Vec const &vec1, Vec const &vec2) {
  return  acos((vec1 * vec2)/ sqrt(vec1.getDoubleMag() * vec2.getDoubleMag()));
}

double Vec::cross (Vec const &vec1, Vec const &vec2) {
  Vec temp;
  //cout << "Vec 1: " << vec1 << "\n";
  //cout << "Vec 2: " << vec2 << "\n";
  if (vec2.getX() > 0) {
    temp = Vec(vec2.getX() * -1, vec2.getY() * -1);
  } else {
    temp = vec2;
  }
  //cout << "Temp: " << temp << "\n";
  //cout << (vec1.getX() * temp.getY()) - (temp.getX() * vec1.getY()) << "\n";
  return (vec1.getX() * temp.getY()) - (temp.getX() * vec1.getY());
}


Vec Vec::operator / (double const &scalar) {
  Vec newVec(this->getX() / scalar, this->getY() / scalar);
  return newVec;
}

Vec Vec::operator / (int const &scalar) {
  Vec newVec(this->getX() / scalar, this->getY() / scalar);
  return newVec;
}

bool Vec::operator ==(Vec const & vec) {
  return this->getX() == vec.getX() && this->getY() == vec.getY();
}

ostream& operator<< (ostream& os, Vec const &vec) {
  os << "<" << vec.getX() << ", " << vec.getY() << ">";
  return os;
}

// int main() {
//   Vec a(4, 3);
//   Vec b(12, 9);
//   cout << a << " " << b <<"\n";
//   Vec c = a + b;
//   cout << c << "\n";
//   Vec d = c * 2;
//   cout << d << "\n";
//
//   double e = a * b;
//   cout << e << "\n";
//
//   double f = Vec::angle(a, b);
//   cout << f*180/3.1415 << "\n";
//
//   double g = Vec::cross(a, b);
//   cout << g << "\n";
// }
