#ifndef PARTICLE_H // include guard
#define PARTICLE_H

#include "utils.h"

using namespace geom;

class Collidable {
public:
  bool is_wall() const;
  virtual int id() const {return 0;}

  virtual void collide() {};
  bool is_wall_;

}; //parent class

class Wall : public Collidable{
public:
  enum Direction { x, y, z }; // absolute direction of normal vector
  Wall () = default;
  Wall(Direction dir);

  vec3 n() const;
  int id() const;

private:
  const Direction dir_;
  vec3 n_;
  int id_;
};

class Particle : public Collidable{
public:
  Particle (int, vec3, vec3, double, double);

  vec3 pos() const;
  vec3 vel() const;
  double m() const;
  double r() const;
  int id() const;
  bool is_wall() const;
  void move(double);
  void validated_move(double, double, double, double);
  bool in_bounds(vec3&, double, double, double) const;

  void collide(Particle&);
  void collide(Wall&);
  void set_vel(vec3);
  void set_pos(vec3);
  vec3 n(Particle&) const;

private:
  vec3 pos_;
  vec3 vel_;
  const int id_;
  const double m_;
  const double r_;
};

#endif
