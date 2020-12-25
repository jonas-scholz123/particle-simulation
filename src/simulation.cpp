#include <iostream>
#include "simulation.h"

int main() {

  int nr_particles {20};

  double max_x {60};
  double max_y {30};
  double max_z {2};

  double max_vel {30};
  double r = 1.0;
  double m = 1.0;

  //ascii animation:
  bool ascii = true;

  double n_seconds = 1000;


  Environment env = Environment(nr_particles, max_x, max_y, max_z, max_vel,
      r, m, ascii);
  //env.test_collision();
  //cout << "number particles in env: " << env.n_part << "\n";
  //cout << "running simulation: " << "\n";

  env.run(n_seconds);
  std::cout << "successfully completed!" <<"\n";
  //cout << "frac of vol occupied: " << nr_particles*4/(15.0*15.0*15.0) << "\n";
  return 1;
}
