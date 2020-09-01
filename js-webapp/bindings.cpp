#include <emscripten/bind.h>
#include "newtonraphson.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(newtonraphson) {
  class_<rootfinding::NewtonRaphson>("NewtonRaphson")
    .constructor<double>()
    .function("solve", &rootfinding::NewtonRaphson::solve)
    .property("iterations", &rootfinding::NewtonRaphson::iterations)
    ;

  value_object<rootfinding::Iteration>("Iteration")
    .field("x", &rootfinding::Iteration::x)
    .field("y", &rootfinding::Iteration::y)
    .field("slope", &rootfinding::Iteration::slope)
    .field("delta_x", &rootfinding::Iteration::delta_x)
    ;
}
