#ifndef H_NEWTONRAPHSON_H
#define H_NEWTONRAPHSON_H

class NewtonRaphson {
  public:
    NewtonRaphson(double tolerance_in);
    double solve(double initial_guess);
  private:
    double tolerance;
};
#endif
