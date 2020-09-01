#include<bits/stdc++.h>
//#include <iomanip>
#include <iostream>

#include "newtonraphson.hpp"
#include <math.h>

int main() {
  double initial_guess = -20;
  double tolerance = 0.001;
  rootfinding::NewtonRaphson finder(tolerance);
  double x1 = finder.solve(initial_guess);

  std::cout << std::fixed;
  std::cout << std::setprecision(6);
  std::cout << "The value of the root is : " << x1 << std::endl;

  for (int i = 0; i < finder.iterations.size(); i++) {
    std::cout << "i = " << i;
    std::cout << " x = " << finder.iterations[i].x;
    std::cout << " y = " << finder.iterations[i].y;
    std::cout << " slope = " << finder.iterations[i].slope;
    std::cout << " delta_x = " << finder.iterations[i].delta_x << std::endl;
  }
  return 0;
}
