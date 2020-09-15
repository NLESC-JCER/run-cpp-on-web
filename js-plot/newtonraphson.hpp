#ifndef H_NEWTONRAPHSON_H
#define H_NEWTONRAPHSON_H

#include <vector>

struct Iteration {
  int index;
  double x;
  double y;
  double slope;
  double delta_x;
};

class NewtonRaphson {
  public:
    NewtonRaphson(double tolerance_in);
    double solve(double initial_guess);
    std::vector<Iteration> iterations;
  private:
    double tolerance;
};
#endif
