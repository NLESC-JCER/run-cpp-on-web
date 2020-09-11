# Show me the visualization

Running C++ code in a web browser is all nice, but we really want to grab someones attention by visualizing something. In this blog we are going to make a plot from the results coming from our C++ code.

To make a plot we need some data. In a [previous](TODO fix url ../js-webapp) post we found the root of an equation using the Newton-Raphson algorithm implemented in C++ and compiled to a WebAsssembly module.
A single root value makes for a depressing plot. The Newton-Raphson algorithm uses iterations to find the root so we will capture the data of each iteration and plot those.

Let's make changes to the C++ code to store the data from the iterations.

## Iterations

To store data of an iteration we will use a structure with the following variables:

* x, x value, starts with initial guess and ends with root result
* y, result of passing x through equation
* slope, slope or derivative at value x
* delta_x, y divided by slope

We will add `iterations` public property to the NewtonRaphson which is a vector of iteration structs. So the `newtonraphson.hpp` becomes

```cpp
#ifndef H_NEWTONRAPHSON_H
#define H_NEWTONRAPHSON_H

#include <vector>

struct Iteration {
  int index;
  double x;
  double y;
  double slope;
  double delta_x;
};

class NewtonRaphson {
  public:
    NewtonRaphson(double tolerance_in);
    double solve(double initial_guess);
    std::vector<Iteration> iterations;
  private:
    double tolerance;
};
#endif
```
File: _newtonraphson.hpp_

The `newtonraphson.cpp` is rewritten from a while loop to a do while loop like with a push to the iterations vector each cycle.

```cpp
#include "newtonraphson.hpp"
#include "algebra.cpp"
#include <math.h>

// Define the constructor method of NewtonRaphson instances
NewtonRaphson::NewtonRaphson(double tolerance_in) : tolerance(tolerance_in) {}

// Define the 'solve' method of NewtonRaphson instances
double NewtonRaphson::solve(double initial_guess) {
  double x = initial_guess;
  double delta_x = 0.0;
  int i = 0;
  do {
    delta_x = equation(x) / derivative(x);
    iterations.push_back({i++, x, equation(x), derivative(x), delta_x});
    x = x - delta_x;
  } while (fabs(delta_x) >= tolerance);
  return x;
};
```
File: _newtonraphson.cpp_.

Before we go into Emscripten world, lets first test our c++ code. We will check if the iteration property is actually populated correctly by wrapping the code in a main function, adding some print statements, compiling it and running it.

```cpp
#include <iostream>
#include <iomanip>

#include "newtonraphson.hpp"

int main() {
  double initial_guess = -3;
  double tolerance = 0.001;
  NewtonRaphson newtonraphson(tolerance);
  newtonraphson.solve(initial_guess);

  std::cout << std::fixed;
  std::cout << std::setprecision(2);

  for (Iteration iteration : newtonraphson.iterations) {
    std::cout << "index = " << iteration.index;
    std::cout << " x = " << iteration.x;
    std::cout << " y = " << iteration.y;
    std::cout << " slope = " << iteration.slope;
    std::cout << " delta_x = " << iteration.delta_x << std::endl;
  }
  return 0;
}
```
File: _cli.cpp_.

Compile it with

```shell
g++ -o cli.exe newtonraphson.cpp cli.cpp
```

Run with

```shell
./cli.exe
index = 0 x = -3.00 y = -84.00 slope = 78.00 delta_x = -1.08
index = 1 x = -1.92 y = -23.02 slope = 37.57 delta_x = -0.61
index = 2 x = -1.31 y = -5.37 slope = 20.79 delta_x = -0.26
index = 3 x = -1.05 y = -0.76 slope = 15.06 delta_x = -0.05
index = 4 x = -1.00 y = -0.03 slope = 14.04 delta_x = -0.00
index = 5 x = -1.00 y = -0.00 slope = 14.00 delta_x = -0.00
```

The last iteration has `-1.00` as x, which is what we expected.

## Bindings

Emscripten can handle simple types like double and int, but needs help exposing more complex types to JavaScript like the iterations property.
We need to use [value_object](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#value-types) to expose the Iteration struct and [register_vector](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#built-in-type-conversions) as the iterations property type.

So the bindings look like

```cpp
#include <emscripten/bind.h>
#include "newtonraphson.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(newtonraphson) {
  class_<NewtonRaphson>("NewtonRaphson")
    .constructor<double>()
    .function("solve", &NewtonRaphson::solve)
    .property("iterations", &NewtonRaphson::iterations)
    ;

  value_object<Iteration>("Iteration")
    .field("index", &Iteration::index)
    .field("x", &Iteration::x)
    .field("y", &Iteration::y)
    .field("slope", &Iteration::slope)
    .field("delta_x", &Iteration::delta_x)
    ;

  register_vector<Iteration>("vector<Iteration>");
}
```
File: _bindings.cpp_.

Same as in previous blog we can compile to a WebAssembly module with Emscripten using `emcc` command

```shell
emcc -I. -o newtonraphson.js -Oz -s MODULARIZE=1 \
  -s EXPORT_NAME=createModule --bind newtonraphson.cpp bindings.cpp
```

## Vega-lite decleration

TODO Introduce Vega

TODO explain vega-spec.json layer construction

```html
<html>
  <head>
    <script type="text/javascript" src="newtonraphson.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/vega@5.13.0"></script>
    <script src="https://cdn.jsdelivr.net/npm/vega-lite@4.13.0"></script>
    <script src="https://cdn.jsdelivr.net/npm/vega-embed@6.8.0"></script>
  </head>
  <body>
    <div id="plot"></div>
    <script>
      createModule().then(async (rootfinding) => {
        const initial_guess = -3;
        const tolerance = 0.001;
        const newtonraphson = new rootfinding.NewtonRaphson(tolerance);
        newtonraphson.solve(initial_guess);
        // newtonraphson.iterations is a vector object which not consumeable by Vega
        // So convert Emscripten vector of objects to JavaScript array of objects
        const iterations = new Array(
            newtonraphson.iterations.size()
        ).fill().map(
          (_, iteration) => {
            return newtonraphson.iterations.get(iteration)
          }
        );
        // Open console in DevTools (F12) to see iterations data as a JSON string
        console.log(JSON.stringify(iterations, null, 2));
        // Read vega-lite declaration
        const spec_request = await fetch('vega-spec.json');
        // TODO decide for spec to be fetched and overwrites or fetched or inlined
        const spec = await spec_request.json();
        // overwrite data
        spec.layer[2].data.values = iterations;
        vegaEmbed(document.getElementById("plot"), spec);
      });
    </script>
  </body>
</html>
```
File: _index.html.

When we visit the web page, we will be greeted by the following plot. We can zoom with mouse wheel and pan by dragging. Also when we hover over a point we get a tooltip with all iteration data.

[![Image](vega-spec.svg)](https://nlesc-jcer.github.io/run-cpp-on-web/js-plot/index.html)
(Click on image to get interactive version)
TODO see if interactive version can be embedded
