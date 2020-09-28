#ifndef H_NEWTONRAPHSON_HPP
#define H_NEWTONRAPHSON_HPP

class NewtonRaphson {
  public:
    NewtonRaphson(float tolerance_in);
    float solve(float initial_guess);
  private:
    float tolerance;
};
#endif
