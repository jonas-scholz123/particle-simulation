#ifndef ASCII_H
#define ASCII_H

#include <vector>
#include <utility>

#include "particle.h"

class Canvas {
public:
  Canvas(double, double);
  void print(std::vector<Particle>&);
private:
  void empty();
  void print_particle(Particle&);
  void print_char(int, int, char);

  std::vector<std::pair<int, int>> occupied;
  std::vector<int> int_vec;
  double max_x;
  double max_y;
  int current_col;
  int n_colors;
  bool use_color;
};

#endif
