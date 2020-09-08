#include "newtonraphson.hpp"
#include "algebra.cpp"
#include <math.h>

// Define the constructor method of NewtonRaphson instances
NewtonRaphson::NewtonRaphson(double tolerance_in) : tolerance(tolerance_in) {}

// Define the 'solve' method of NewtonRaphson instances
double NewtonRaphson::solve(double initial_guess) {
  double x = initial_guess;
  double delta_x = equation(x) / derivative(x);

  while (fabs(delta_x) >= tolerance) {
    delta_x = equation(x) / derivative(x);
    x = x - delta_x;
  }
  return x;
};
