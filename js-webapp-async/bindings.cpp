#include <emscripten/bind.h>
#include "newtonraphson.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(newtonraphson) {
   class_<rootfinding::NewtonRaphson>("NewtonRaphson")
      .constructor<double>()
      .function("solve", &rootfinding::NewtonRaphson::solve)
      ;
}
