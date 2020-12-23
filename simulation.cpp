#include <iostream>
#include <random>
#include <queue>

using namespace std;

#include "Environment.h"
#include "Particle.h"
#include "curses.h"



int main() {
  int nr_particles {20};

  double max_y {30};
  double max_x {60};
  double max_z {2};


  Environment env = Environment(nr_particles, max_x, max_y, max_z);
  //env.test_collision();
  //cout << "number particles in env: " << env.n_part << "\n";
  //cout << "running simulation: " << "\n";

  env.run(100000);
  cout << "successfully completed!" <<"\n";
  //cout << "frac of vol occupied: " << nr_particles*4/(15.0*15.0*15.0) << "\n";
  return 1;
}
