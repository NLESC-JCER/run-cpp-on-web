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
  double initial_guess = 2;
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
index = 0 x = 2.00 y = 6.00 slope = 8.00 delta_x = 0.75
index = 1 x = 1.25 y = 3.66 slope = -0.62 delta_x = -5.85
index = 2 x = 7.10 y = 520.18 slope = 245.66 delta_x = 2.12
index = 3 x = 4.98 y = 154.08 slope = 109.09 delta_x = 1.41
index = 4 x = 3.57 y = 46.02 slope = 47.91 delta_x = 0.96
index = 5 x = 2.61 y = 14.30 slope = 19.98 delta_x = 0.72
index = 6 x = 1.89 y = 5.24 slope = 6.37 delta_x = 0.82
index = 7 x = 1.07 y = 3.87 slope = -1.68 delta_x = -2.30
index = 8 x = 3.37 y = 37.03 slope = 41.11 delta_x = 0.90
index = 9 x = 2.47 y = 11.69 slope = 16.78 delta_x = 0.70
index = 10 x = 1.77 y = 4.56 slope = 4.65 delta_x = 0.98
index = 11 x = 0.79 y = 4.49 slope = -2.58 delta_x = -1.74
index = 12 x = 2.53 y = 12.83 slope = 18.22 delta_x = 0.70
index = 13 x = 1.83 y = 4.85 slope = 5.43 delta_x = 0.89
index = 14 x = 0.93 y = 4.14 slope = -2.24 delta_x = -1.85
index = 15 x = 2.79 y = 18.20 slope = 24.28 delta_x = 0.75
index = 16 x = 2.04 y = 6.30 slope = 8.59 delta_x = 0.73
index = 17 x = 1.30 y = 3.63 slope = -0.24 delta_x = -15.17
index = 18 x = 16.47 y = 7859.08 slope = 1496.16 delta_x = 5.25
index = 19 x = 11.22 y = 2326.73 slope = 665.45 delta_x = 3.50
index = 20 x = 7.72 y = 688.55 slope = 296.04 delta_x = 2.33
index = 21 x = 5.40 y = 203.85 slope = 131.57 delta_x = 1.55
index = 22 x = 3.85 y = 60.69 slope = 58.03 delta_x = 1.05
index = 23 x = 2.80 y = 18.58 slope = 24.68 delta_x = 0.75
index = 24 x = 2.05 y = 6.41 slope = 8.79 delta_x = 0.73
index = 25 x = 1.32 y = 3.63 slope = -0.11 delta_x = -33.86
index = 26 x = 35.18 y = 82124.47 slope = 7143.73 delta_x = 11.50
index = 27 x = 23.68 y = 24327.61 slope = 3175.69 delta_x = 7.66
index = 28 x = 16.02 y = 7204.88 slope = 1412.03 delta_x = 5.10
index = 29 x = 10.92 y = 2132.99 slope = 628.05 delta_x = 3.40
index = 30 x = 7.52 y = 631.21 slope = 279.40 delta_x = 2.26
index = 31 x = 5.26 y = 186.90 slope = 124.15 delta_x = 1.51
index = 32 x = 3.76 y = 55.69 slope = 54.70 delta_x = 1.02
index = 33 x = 2.74 y = 17.12 slope = 23.14 delta_x = 0.74
index = 34 x = 2.00 y = 6.00 slope = 8.01 delta_x = 0.75
index = 35 x = 1.25 y = 3.66 slope = -0.62 delta_x = -5.89
index = 36 x = 7.14 y = 530.98 slope = 249.04 delta_x = 2.13
index = 37 x = 5.01 y = 157.28 slope = 110.60 delta_x = 1.42
index = 38 x = 3.59 y = 46.96 slope = 48.59 delta_x = 0.97
index = 39 x = 2.62 y = 14.58 slope = 20.30 delta_x = 0.72
index = 40 x = 1.91 y = 5.31 slope = 6.54 delta_x = 0.81
index = 41 x = 1.09 y = 3.83 slope = -1.58 delta_x = -2.43
index = 42 x = 3.52 y = 43.71 slope = 46.21 delta_x = 0.95
index = 43 x = 2.58 y = 13.63 slope = 19.19 delta_x = 0.71
index = 44 x = 1.86 y = 5.06 slope = 5.95 delta_x = 0.85
index = 45 x = 1.01 y = 3.97 slope = -1.94 delta_x = -2.04
index = 46 x = 3.06 y = 25.82 slope = 31.67 delta_x = 0.82
index = 47 x = 2.24 y = 8.45 slope = 12.26 delta_x = 0.69
index = 48 x = 1.55 y = 3.85 slope = 2.06 delta_x = 1.87
index = 49 x = -0.32 y = 5.54 slope = 3.12 delta_x = 1.78
index = 50 x = -2.09 y = -29.80 slope = 42.98 delta_x = -0.69
index = 51 x = -1.40 y = -7.29 slope = 22.92 delta_x = -0.32
index = 52 x = -1.08 y = -1.19 slope = 15.64 delta_x = -0.08
index = 53 x = -1.00 y = -0.06 slope = 14.09 delta_x = -0.00
index = 54 x = -1.00 y = -0.00 slope = 14.00 delta_x = -0.00
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
        const initial_guess = 2;
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
