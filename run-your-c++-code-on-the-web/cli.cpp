#include <iostream>
#include <iomanip>

#include "newtonraphson.hpp"

int main() {
  float initial_guess = -4;
  float tolerance = 0.001;
  NewtonRaphson newtonraphson(tolerance);
  float root = newtonraphson.solve(initial_guess);

  std::cout << "Function root is approximately at x = ";
  std::cout << std::fixed << std::setprecision(2) << root << std::endl;

  return 0;
}
