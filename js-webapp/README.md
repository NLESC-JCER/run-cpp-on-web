# C++ in a web browser

\<paint picture of how cool we'd be>

\<describe the payoff>

So the aim is to have a simple web app like the code snippet below. At first glance this is just some boilerplate to be able to use the ``newtonraphson.js`` library, but **the neat part is that the complete Newton-Raphson code is written in C++, not JavaScript**. However, with some trickery we'll be able to use that C++ code from the browser, without the need to port it first!

```html
<!doctype html>
<html lang="en">
  <head>
    <title>Example</title>
    <script type="text/javascript" src="newtonraphson.js"></script>
    <script>
      const module = await createModule();
      const tolerance = 0.001;
      const newtonraphson = new module.NewtonRaphson(tolerance);
      const initial_guess = -20;
      const root = newtonraphson.solve(initial_guess);
      document.getElementById('answer').innerHTML = root.toFixed(2);
    </script>
  </head>
  <body>
    <span id="answer"> </span>
  </body>
</html>
```

\<be sceptic>

![hold-your-horses.jpeg](hold-your-horses.jpeg)
_Hold your horses._

Now before you say _"That'll be so much slower than running it native!"_ or _"C/C++ from the browser? Impossible!"_, just hold your horses for a sec. With the right tools, something something.

\<Let's get to it>

OK, now that you're fully on board with this, let's get to it. 

## What we'll need

1. Some C/C++ code. We'll use some C++ code that implements the _Newton-Raphson_ root finding method (You know Newton? Quiet fellow, fabulous hair? Yes, him). In case you didn't know, Newton-Raphson will find the root of a mathematical function, i.e. the value of x where it crosses y.
1. A program that can take our existing C/C++ code and compile it into a WebAssembly module. Modern browsers are able to run WebAssembly without loss of performance. For this, we'll use [Emscripten](https://emscripten.org/)'s ``emcc`` compiler, the most popular C++ to WebAssembly compiler of the bunch.
1. To call C++ code (which has been compiled to a WebAssembly module) from JavaScript, a binding is required. The binding will map C++ constructs to their JavaScript equivalent and back. For this, we'll use [embind](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind).
1. A web server to serve our files. We'll use Python 3's ``http.server``, but other options like X and Y work equally well.


## Tying it all together

<!--
Basically, take the C++ and the bindings, then use emcc to compile it into wasm, then call the result from JS. Show the required code snippets
-->

### The C++ code

The Newton-Raphson header file:

```cpp
#ifndef H_NEWTONRAPHSON_H
#define H_NEWTONRAPHSON_H

#include <string>

namespace rootfinding {
  class NewtonRaphson {
    public:
      NewtonRaphson(double tolerancein);
      double solve(double xin);
    private:
      double tolerance;
  };
}

#endif
```
File: _newtonraphson.hpp_

The Newton-Raphson source file:

```cpp
#include "newtonraphson.hpp"
#include "algebra.hpp"
#include <math.h>

using namespace algebra;

namespace rootfinding
{

NewtonRaphson::NewtonRaphson(double tolerancein) : tolerance(tolerancein) {}

// Function to find the root
double NewtonRaphson::solve(double xin)
{
  double x = xin;
  double delta_x = equation(x) / derivative(x);

  while (fabs(delta_x) >= tolerance)
  {
    delta_x = equation(x) / derivative(x);
    x = x - delta_x;
  }
  return x;
};
}
```
File: _newtonraphson.cpp_

The binding of the C++ code:

```cpp
#include <emscripten/bind.h>
#include "newtonraphson.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(newtonraphsonwasm) {
  class_<rootfinding::NewtonRaphson>("NewtonRaphson")
    .constructor<double>()
    .function("solve", &rootfinding::NewtonRaphson::solve)
    ;
}
```

The Newton-Raphson source and its binding can be compiled into a WebAssembly module with Emscripten's ``emcc`` compiler, as follows:

```shell
emcc -I. -o newtonraphson.js \
  -Oz -s MODULARIZE=1 -s EXPORT_NAME=createModule \
  --bind newtonraphson.cpp wasm-newtonraphson.cpp
```

This will generate a JavaScript file, ``newtonraphson.js``, that we can embed in an HTML page, as follows:


```html
<!doctype html>
<html lang="en">
  <head>
    <title>Example</title>
    <script type="text/javascript" src="newtonraphson.js"></script>
    <script>
      const module = await createModule();
      const tolerance = 0.001;
      const newtonraphson = new module.NewtonRaphson(tolerance);
      const initial_guess = -20;
      const root = newtonraphson.solve(initial_guess);
      document.getElementById('answer').innerHTML = root.toFixed(2);
    </script>
  </head>
  <body>
    <span id="answer"> </span>
  </body>
</html>
```
_example.html_

When this page is loaded, ... (explain what's happening)
The last step is to render the answer on the page using the document manipulation method [``getElementById``](https://developer.mozilla.org/en-US/docs/Web/API/Document/getElementById).

We can not just open this HTML page in a web browser, as the embedded JavaScript file can only be loaded when it is hosted by a web server. Python3 ships with a built-in web server ``http.server``, we will use it to host all files on port 8000.

```shell
python3 -m http.server 8000
```

Visit [http://localhost:8000/example.html](http://localhost:8000/example.html) to see the result of the calculation. 

(Show a picture of what it looks like.)
Caption _When everything went OK we should see a page with `-1.00` which is the correct answer for the root of the defined equation given the initial guess of -20._

(Recap and announce what else is coming)

The result of root finding was calculated using the C++ algorithm compiled to a WebAssembly module, executed by some JavaScript and rendered on a HTML page.

The nice thing about this solution is that we don't need expensive infrastructure to perform computation as the computation is done in the users web browser. We just need somewhere to host the files.

In upcoming blogs will see if we can perform the computation without blocking the user interface and make a nice interactive form. In even more blogs we will look into performing the computation on the server with JavaScript and Python in a human readable and compute readable format.

(Good practice to activate users by asking for claps--I you don't, they won't).

If you enjoyed this article, make sure to give us clap!
