#ifndef ENVIRONMENT_H // include guard
#define ENVIRONMENT_H

#include <vector>
#include <array>
#include <random>
#include <tuple>
#include <queue>
#include <map>

#include <curses.h>
#include "Particle.h"
#include "ascii.h"

class Collision {
public:
  Collision () = default;
  Collision (double, double, Collidable&, Collidable&, int, int);

  double t() const {return t_;}
  int id_1() const {return id_1_;}
  int id_2() const {return id_2_;}
  int n_col_1() const {return n_col_1_;}
  int n_col_2() const {return n_col_2_;}

  bool is_valid(std::vector<int>) const;
  bool is_wall() const;

private:
  double t_;
  int id_1_;
  int id_2_;
  int n_col_1_;
  int n_col_2_;
  bool is_wall_;
};

//for ordering priority queue
bool operator<(const Collision& lhs, const Collision& rhs);

class Environment {
public:
  typedef std::tuple<double, int, int> myCol;
  Environment (int, double, double, double);

  void run(int);
private:

  double max_x, max_y, max_z;
  double max_vel;
  int n_part;
  double time;
  std::vector<int> n_cols;
  double t_frame;
  double r;
  double m;
  double post_collision_dt;
  bool ascii;
  Canvas canvas;
  std::array<Wall, 3> walls;

  std::uniform_real_distribution<> dist;
  std::mt19937 gen;
  std::priority_queue<Collision> collisions;
  std::vector<Particle> particles;

  void add_particle(int, vec3, vec3);
  void add_random_particle(int);
  vec3 get_random_pos();
  vec3 get_random_vel();

  void init_collisions();

  void tick();
  void move(double);
  double t2col(Particle&, Particle&);
  void update_collisions_involving(Particle&);
  void collide (Particle&, Particle&);
  void collide (Particle&, Wall&);
  void add_wall_collisions(Particle&);
};

#endif
