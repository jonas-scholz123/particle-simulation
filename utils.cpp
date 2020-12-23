#include "utils.h"
#include <iostream>

#include <cmath>
#include "curses.h"

using namespace geom;

vec3::vec3() : x_{0}, y_{0}, z_{0} {}
vec3::vec3(double x, double y, double z) :
  x_{x}, y_{y}, z_{z} {}

double vec3::x() const {return x_;}
double vec3::y() const {return y_;}
double vec3::z() const {return z_;}

void vec3::print() {
  std::cout << "("<< x_ << " " << y_ << " " << z_  << ")\n";
}

vec3& vec3::operator *= (const double& scale) {
  x_ *= scale;
  y_ *= scale;
  z_ *= scale;
return *this;
}
vec3& vec3::operator /= (const double& scale) {
  x_ /= scale;
  y_ /= scale;
  z_ /= scale;
return *this;
}

vec3& vec3::operator += (const vec3& other) {
  x_ += other.x_;
  y_ += other.y_;
  z_ += other.z_;
return *this;
}
vec3& vec3::operator -= (const vec3& other) {
  x_ -= other.x_;
  y_ -= other.y_;
  z_ -= other.z_;
return *this;
}

double vec3::dot(const vec3& other) const {
    double sp;
    sp = x_*other.x() + y_*other.y() + z_*other.z();
    return sp;
}

double vec3::size() const {
  return std::sqrt(x_*x_ + y_*y_ + z_*z_);
}

vec3 operator + (const vec3& lhs, const vec3& rhs){
  return vec3( lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z() );
}

vec3 operator - (const vec3& lhs, const vec3& rhs){
  return vec3( lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z() );
}

vec3 operator * (const double lhs, const vec3& rhs){
  return vec3(lhs*rhs.x(), lhs*rhs.y(), lhs*rhs.z());
}

vec3 operator / (const vec3& lhs, const double& rhs){
  return vec3(lhs.x()/rhs, lhs.y()/rhs, lhs.z()/rhs);
}
