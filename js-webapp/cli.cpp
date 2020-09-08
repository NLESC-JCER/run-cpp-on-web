#include <iostream>
#include <iomanip>

#include "newtonraphson.cpp"

int main() {
  double initial_guess = -20;
  double tolerance = 0.001;
  NewtonRaphson newtonraphson(tolerance);
  double root = newtonraphson.solve(initial_guess);

  std::cout << std::fixed;
  std::cout << std::setprecision(2);
  std::cout << "Function root is approximately at x = " << root << std::endl;

  return 0;
}
