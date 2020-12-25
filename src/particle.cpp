#include "particle.h"

Particle::Particle (int id, vec3 pos, vec3 vel, double m, double r) :
  pos_ {pos},
  vel_ {vel},
  id_ {id},
  m_ {m},
  r_ {r}

  {is_wall_ = false;}

vec3 Particle::pos() const {return pos_;}
vec3 Particle::vel() const {return vel_;}
double Particle::m() const {return m_;}
double Particle::r() const {return r_;}
int Particle::id() const {return id_;}

void Particle::set_vel(vec3 vel) {vel_ = vel;}
void Particle::set_pos(vec3 pos) {pos_ = pos;}

void Particle::collide(Particle& other){
  const vec3& u1 = this -> vel();
  const vec3& u2 = other.vel();
  const vec3& u = u1 - u2; //relative velocity
  const vec3 n = this -> n(other); //normal vector

  const vec3& u_norm = u.dot(n) * n;

  this -> set_vel(u1 - u_norm);
  other.set_vel(u2 + u_norm);
}

void Particle::collide(Wall& w){
  vec3 norm_vel = vel_.dot(w.n()) * w.n();
  vel_ -= 2 * norm_vel;
}

vec3 Particle::n(Particle& other) const {
  const vec3 dif = (this -> pos()) - (other.pos());
  return dif / dif.size();
}

bool Collidable::is_wall() const {
  return is_wall_;
}

Wall::Wall (Direction dir) :
  dir_ (dir)
  {
    is_wall_ = true;

    if (dir == x) {
      id_ = 0;
      n_ = vec3(1, 0, 0);
    }
    else if (dir == y) {
      id_ = 1;
      n_ = vec3(0, 1, 0);
    }
    else {
      id_ = 2;
      n_ = vec3(0, 0, 1);
    }
  }

vec3 Wall::n() const {return n_;}

int Wall::id() const {return id_;}

void Particle::move(double dt) {
  pos_ += dt* vel_;
}
bool Particle::in_bounds(vec3& pos, double max_x, double max_y, double max_z) const {
  return (pos.x() <= max_x - r_ && pos.x() >= r_
    && pos.y() <= max_y - r_ && pos.y() >= r_
    && pos.z() <= max_z -r_ && pos.z() >= r_);
}

void Particle::validated_move(double dt, double max_x, double max_y, double max_z) {
  //checks if in bounds after move for "unphysical" moves
  auto new_pos = pos_ + dt * vel_;
  if (in_bounds(new_pos, max_x, max_y, max_z)){
      pos_ = new_pos;
    }
}
