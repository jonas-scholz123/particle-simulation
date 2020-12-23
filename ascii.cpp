#include "ascii.h"
#include "Particle.h"
#include <iostream>

#include <cmath>
#include "curses.h"

Canvas::Canvas(double max_x, double max_y) :
  max_x {max_x},
  max_y {max_y},
  current_col {0},
  n_colors {6},
  use_color {true}

  {

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    if (use_color) {
      start_color();
      init_pair(0, COLOR_WHITE, COLOR_BLACK);
      init_pair(1, COLOR_RED, COLOR_BLACK);
      init_pair(2, COLOR_GREEN, COLOR_BLACK);
      init_pair(3, COLOR_BLUE, COLOR_BLACK);
      init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
      init_pair(5, COLOR_CYAN, COLOR_BLACK);
    }

    clear();
    for (int i=0; i<max_x + 2; ++i) {
      mvaddch(0, i, '=');
      mvaddch(max_y + 2, i, '=');
    }

    for (int i=0; i<max_y + 2; ++i) {
      mvaddch(i, 0, '|');
      mvaddch(i, max_x + 2, '|');
    }
  }

void Canvas::print(std::vector<Particle>& particles) {
  empty();
  for (auto& p : particles) {
    print_particle(p);
  }
  refresh();
}

void Canvas::print_particle(Particle& p) {

  int x = std::round(p.pos().x()) + 1; // + 1 because of boarder chars
  int y = std::round(p.pos().y()) + 1;

  if (use_color) {
    attron(COLOR_PAIR(p.id() % n_colors));
  }
  if (x <= max_x && x >= 0 && y <= max_y && y >= 0){
    print_char(y, x, 'o');
    print_char(y-1, x, '_');
    print_char(y+1, x, '"');
    print_char(y, x + 1, ')');
    print_char(y, x - 1, '(');
  }
  if (use_color) {
    attroff(COLOR_PAIR(p.id() % n_colors));
  }
  ++ current_col;
}

void Canvas::empty(){
  while (!occupied.empty()){
    //std::cout << "len occ: "<< occupied.size() << "\n";
    auto& xy = occupied.back();
    auto& x = std::get<0>(xy);
    auto& y = std::get<1>(xy);
    mvaddch(y, x, ' ');
    occupied.pop_back();
  }
}

void Canvas::print_char(int y, int x, char c) {
  mvaddch(y, x, c);
  occupied.push_back(std::make_pair(x, y));
}
