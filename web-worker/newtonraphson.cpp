#include "newtonraphson.hpp"
#include "problem.hpp"
#include <cmath>
#include <unistd.h> // for sleep

// Define the constructor method of NewtonRaphson instances
NewtonRaphson::NewtonRaphson(float tolerance_in) : tolerance(tolerance_in) {}

// Define the 'solve' method of NewtonRaphson instances
float NewtonRaphson::solve(float initial_guess) {
  float x = initial_guess;
  float delta_x = 0;
  do {
    delta_x = equation(x) / derivative(x);
    x = x - delta_x;
  } while (std::abs(delta_x) >= tolerance);

  // Artificially make this code slow
  sleep(5);

  return x;
};
