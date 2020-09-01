#ifndef H_NEWTONRAPHSON_H
#define H_NEWTONRAPHSON_H

#include <string>
#include <vector>

namespace rootfinding {

  struct Iteration {
    double x;
    double y;
    double slope;
    double delta_x;
  };

  class NewtonRaphson {
    public:
      NewtonRaphson(double tolerancein);
      double solve(double xin);
      std::vector<Iteration> iterations;
    private:
      double tolerance;
  };
}
#endif
