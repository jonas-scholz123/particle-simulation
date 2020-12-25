#include "environment.h"

#include <algorithm>
#include <unistd.h> //usleep() for animation

Collision::Collision (double t0, double dt, Collidable& p1, Collidable& p2,
  int n_col_1, int n_col_2):
  t_ {t0 + dt},
  id_1_ {p1.id()},
  id_2_ {p2.id()},
  n_col_1_ {n_col_1},
  n_col_2_ {n_col_2}
  {
    is_wall_ = p2.is_wall();
  }

bool Collision::is_valid (std::vector<int> n_cols) const {
  //wall collisions only need to check particle
  if (is_wall_){
    return n_cols[id_1_] == n_col_1_;
  }
  return (n_cols[id_1_] == n_col_1_ && n_cols[id_2_] == n_col_2_);
}

bool Collision::is_wall () const {
  return is_wall_;
}

bool operator< (const Collision& lhs, const Collision& rhs) {
  return lhs.t() > rhs.t(); //means priority_queue is ascending
}

Environment::Environment (int n_part, double max_x, double max_y, double max_z,
   double max_vel, double r, double m, bool ascii):
  max_x {max_x},
  max_y {max_y},
  max_z {max_z},
  max_vel {max_vel},
  n_part {n_part},
  time {0},
  n_cols(n_part, 0),
  t_frame {0.01},
  r {1.0},
  m {1.0},
  post_collision_dt {0.0001},
  ascii {true},
  canvas {Canvas(max_x, max_y)},
  walls {Wall(Wall::x), Wall(Wall::y), Wall(Wall::z)},
  //gen {std::mt19937()}, //already declared in .h
  dist {std::uniform_real_distribution<>(0, 1)}
  {
    for (int i; i<n_part; ++i)
    add_random_particle(i);
  }

void Environment::run(int T) {
  init_collisions();
  while (time < T){
    tick();
  }
}

void Environment::add_particle(int id, vec3 pos, vec3 vel){
  particles.push_back(Particle(id, pos, vel, m, r));
}

void Environment::add_random_particle(int id){
  vec3 pos {Environment::get_random_pos()};
  vec3 vel = Environment::get_random_vel();
  vel.print();
  Environment::add_particle(id, pos, vel);
}

vec3 Environment::get_random_pos() {
  double x = (max_x - 2*r) * dist(gen) + r;
  double y = (max_y - 2*r) * dist(gen) + r;
  double z = (max_z - 2*r) * dist(gen) + r;

  //std::cout << "RNG: " << dist(gen) << "\n";
  return vec3(x, y, z);
}

vec3 Environment::get_random_vel() {
  double x = max_vel * dist(gen);
  double y = max_vel * dist(gen);
  //double z = max_vel * dist(gen);
  double z = 0.0; //temporary for animation
  return vec3(x, y, z);
}

void Environment::init_collisions() {

  for (int i = 0; i < n_part; ++i){

    auto& p1 = particles[i];
    add_wall_collisions(p1);

    for (int j = 0; j < i; ++j){
      auto& p2 = particles[j];
      double dt = t2col(p1, p2);

      //if valid t2col
      if (dt > 0) {
        //0 = init nr collisions
        Collision col = Collision(time, dt, p1, p2, 0, 0);
        collisions.push(col);
      }
    }
  }
}

void Environment::tick() {
  const Collision& next_col = collisions.top();

  // fetch next col from queue, check if valid
  if (next_col.is_valid(n_cols)) {
    const auto& dt = next_col.t() - time;
    const auto& cid1 = next_col.id_1();
    const auto& cid2 = next_col.id_2();
    auto& col_p1 = particles[cid1];
    auto& col_p2 = particles[cid2];

    if (ascii) {
      if (dt >= t_frame) {
        move(t_frame);
        canvas.print(particles); //prints the simulation in ascii style
        usleep(t_frame*1000*1000); //sleep to have animation be slow enough
        return;
      }
    }
    //move to location of collision
    move(dt);

    if (next_col.is_wall()) {
      //collide particle with wall
      collide(col_p1, walls[cid2]);
    }
    else {
      //collide particles
      collide(col_p1, col_p2);
    }
  }
  //remove recent collision from queue
  collisions.pop();
}

void Environment::move(double dt) {
  for (auto &p : particles) {
    p.move(dt);
  }
  time += dt;
}

double Environment::t2col(Particle& p1, Particle& p2) const {
  vec3 x = p1.pos() - p2.pos();
  vec3 v = p1.vel() - p2.vel();

  const double vv = v.dot(v);
  const double xv = x.dot(v);
  const double R = p1.r() + p2.r();

  const double radical = xv*xv - vv*(x.dot(x) - R*R);
  if (radical >= 0) {
    const double t2col = (-xv - std::sqrt(radical))/vv;
    if (t2col < 0){
      return -1;
    }
    else{
      return t2col;
    }
  }
  else{
    return -1;
  }
}

void Environment::update_collisions_involving(Particle& p) {
  for (auto& p2 : particles) {
    const auto& dt = t2col(p, p2);
    if (dt > 0) {
      auto col = Collision(time, dt, p, p2, n_cols[p.id()], n_cols[p2.id()]);
      collisions.push(col);
    }
  }

  add_wall_collisions(p);
}

void Environment::collide(Particle& p1, Particle& p2) {
  p1.collide(p2);
  n_cols[p1.id()]++;
  n_cols[p2.id()]++;

  //move a tiny amount post collision so that particles don't clip together
  p1.validated_move(post_collision_dt, max_x, max_y, max_z);
  p2.validated_move(post_collision_dt, max_x, max_y, max_z);

  update_collisions_involving(p1);
  update_collisions_involving(p2);
}

void Environment::collide(Particle& p, Wall& w) {
  n_cols[p.id()]++;
  // wall_id -1:x => vel x -> - vel_x etc
  p.collide(w);
  update_collisions_involving(p);
}

void Environment::add_wall_collisions(Particle& p) {

  //get lowest collision time
  double t_x;
  if (p.vel().x() > 0){
     t_x = (max_x - p.r() - p.pos().x())/p.vel().x();
   }
  else {
    t_x = -(p.pos().x() - p.r())/p.vel().x();
  }

  double t_y;
  if (p.vel().y() > 0){
     t_y = (max_y - p.r() - p.pos().y())/p.vel().y();
   }
  else {
    t_y = -(p.pos().y() - p.r())/p.vel().y();
  }

  double t_z;
  if (p.vel().z() > 0){
     t_z = (max_z - p.r() - p.pos().z())/p.vel().z();
   }
  else {
    t_z = -(p.pos().z() - p.r())/p.vel().z();
  }

  auto t_min = std::min({t_x, t_y, t_z});

  Collision col;
  // add collision event
  if (t_min==t_x) {
    col = Collision(time, t_x, p, walls[0], n_cols[p.id()], -1);
  }
  else if (t_min==t_y) {
    col = Collision(time, t_y, p, walls[1], n_cols[p.id()], -1);
  }
  else {
    col = Collision(time, t_z, p, walls[2], n_cols[p.id()], -1);
  }
  collisions.push(col);
}
