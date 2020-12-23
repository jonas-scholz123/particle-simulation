#ifndef UTILS_H
#define UTILS_H

#include <vector>

namespace geom{
  class vec3 {
  public:
    vec3();
    vec3(double x, double y, double z);

    double x() const;
    double y() const;
    double z() const;

    double size() const;

    vec3& operator *= (const double&);
    vec3& operator /= (const double&);
    vec3& operator += (const vec3&);
    vec3& operator -= (const vec3&);


    double dot(const vec3&) const;

    void print();

  private:
    double x_, y_, z_;

  };

}

  geom::vec3 operator + (const geom::vec3& lhs, const geom::vec3& rhs);

  geom::vec3 operator - (const geom::vec3& lhs, const geom::vec3& rhs);

  geom::vec3 operator * (const double lhs, const geom::vec3& rhs);

  //geom::vec3 operator / (const geom::vec3& lhs, const double& rhs);
  geom::vec3 operator / (const geom::vec3& lhs, const double& rhs);
#endif
