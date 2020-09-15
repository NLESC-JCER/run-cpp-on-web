#ifndef H_NEWTONRAPHSON_HPP
#define H_NEWTONRAPHSON_HPP

#include <vector>

struct Iteration {
  int index;
  float x;
  float y;
  float slope;
  float delta_x;
};

class NewtonRaphson {
  public:
    NewtonRaphson(float tolerance_in);
    float solve(float initial_guess);
    std::vector<Iteration> iterations;
  private:
    float tolerance;
};
#endif
