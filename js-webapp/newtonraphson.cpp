#include "newtonraphson.hpp"
#include "algebra.cpp"
#include <math.h>

// Define the constructor method of NewtonRaphson instances
NewtonRaphson::NewtonRaphson(double tolerancein) : tolerance(tolerancein) {}

// Define the 'solve' method of NewtonRaphson instances
double NewtonRaphson::solve(double xin) {
  double x = xin;
  double delta_x = equation(x) / derivative(x);

  while (fabs(delta_x) >= tolerance) {
    delta_x = equation(x) / derivative(x);
    x = x - delta_x;
  }
  return x;
};
