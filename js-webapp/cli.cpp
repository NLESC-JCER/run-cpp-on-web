#include <iostream>
#include <iomanip>

#include "newtonraphson.hpp"

int main() {
  double initial_guess = -20;
  double tolerance = 0.001;
  NewtonRaphson finder(tolerance);
  double x1 = finder.solve(initial_guess);
  std::cout << "Function root is approximately at x = ";
  std::cout << std::fixed << std::setprecision(2) << x1 << std::endl;
  return 0;
}
