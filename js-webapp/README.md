# Run your C++ code on the web

Let's say you have some C or C++ code laying around that you would like to make available to a wider audience, by putting
it on the web. Until recently, this used to be pretty difficult, and may even have required reimplementation of the
software in JavaScript, the programming language that browsers use.

_Wouldn't it be great if you could run your existing C/C++ code on the web with only minor effort?_

That way, loads more people would be able to see your results, interact with your algorithm, and apply it for their own
purposes.

In this blog, we'll show you how to take a simple algorithm written in C++ and make it available as a web
application. Subsequent blogs in this series will expand on the current one by laying out more advanced topics,
specifically how to [make the app interactive](anotherblog), how to [visualize the results](someotherblog), and
how to [deal with long running tasks](yetanotherblog).

So today's aim is to have a simple web app that determines the root of a mathematical function _x^3 - x^2 + 2_, i.e. the
value of _x_ where _y = 0_.

![equation.svg.png](equation.svg.png)

Function _x^3 - x^2 + 2_.

For this, we'll use an iterative method known as the [_Newton-Raphson_ root finding
method](https://www.youtube.com/watch?v=cOmAk82cr9M). Remember Newton? Quiet fellow, fabulous hair? Yes, _that_ Newton.
The way this works is you give Newton-Raphson the equation whose root you want to find, along with the derivative of
that equation. Then you take an ``initial_guess`` of what you think the value of the root could be, then let the method
iterate towards the solution. The solution is approximate within a ``tolerance``, which you can also set. Anyway, the
algorithm is written C++, but **with some trickery, we'll be able to use that C++ code from the browser, without the
need to port it first**!


![newton.jpg](newton.jpg)

_Newton (and his hair)._

Now before you say _"That'll be so much slower than running it native!"_ or _"C/C++ from the browser? Impossible!"_,
just hold your horses for a sec. With the right tools, it is possible to run C/C++ code in the browser, without any
significant performance penalty. WebAssembly to the rescue, a low level language browsers can run and C++ can be compiled to. Using this approach, Gabriel Cuvillier was able to run the video
game [Doom 3 in the browser](http://wasm.continuation-labs.com/d3demo/) and didn't find any performance problems. And if it works for video games, it will likely work
for your research software, too.

![hold-your-horses.jpeg](hold-your-horses.jpeg)

_Hold your horses._

## What we'll need

OK, now that you're fully on board with this, let's get to it. Here's a list of what we need:

1. We are going to write a small HTML page, so you will need basic knowledge about [HTML pages](https://developer.mozilla.org/en-US/docs/Learn/Getting_started_with_the_web/HTML_basics) and the programming language embedded in each web browser called [JavaScript](https://developer.mozilla.org/en-US/docs/Web/JavaScript) to make HTML pages interactive.
1. Some C/C++ code to illustrate the process. We'll use our Newton-Raphson C++ code.
1. A program that can take our existing C/C++ code and compile it into a WebAssembly module. Modern browsers are able to
run WebAssembly without loss of performance. For this, we'll use [Emscripten](https://emscripten.org/)'s ``emcc``
compiler, the most popular C++ to WebAssembly compiler of the bunch.
1. To use the WebAssembly functionality from JavaScript, a binding is required. The binding will map C++ constructs to
their JavaScript equivalent and back. For this, we'll use
[embind](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind).
1. A web server to serve our files. We'll use Python 3's ``http.server``, but other options like X and Y work equally well.

## Tying it all together

### The C++ code

Here is the equation whose root we want to find, along with its derivative, since that's what Newton-Raphson requires:

```cpp
// An example equation
double equation(double x) {
  return x * x * x - x * x + 2;
}

// Derivative of the above equation
double derivative(double x) {
  return 3 * x * x - 2 * x;
}
```
File: _algebra.cpp_

The snippet below shows the contents of the file ``newtonraphson.hpp``. It is the header file for the Newton-Raphson
iterative root finding algorithm. It defines a class ``NewtonRaphson`` in namespace ``rootfinding``. Besides the
constructor method ``NewtonRaphson(double tolerance_in)``, ``NewtonRaphson`` has one other public method, ``solve``,
which takes a ``double``, and returns another ``double``. Furthermore, ``NewtonRaphson`` also has a private member,
``tolerance`` of type ``double``, which is used to store the class instance's private data.

```cpp
#ifndef H_NEWTONRAPHSON_H
#define H_NEWTONRAPHSON_H

class NewtonRaphson {
  public:
    NewtonRaphson(double tolerance_in);
    double solve(double initial_guess);
  private:
    double tolerance;
};
#endif
```
File: _newtonraphson.hpp_.

File ``newtonraphson.cpp`` contains the corresponding implementation:

```cpp
#include "newtonraphson.hpp"
#include "algebra.cpp"
#include <math.h>

// Define the constructor method of NewtonRaphson instances
NewtonRaphson::NewtonRaphson(double tolerance_in) : tolerance(tolerance_in) {}

// Define the 'solve' method of NewtonRaphson instances
double NewtonRaphson::solve(double initial_guess) {
  double x = initial_guess;
  double delta_x = equation(x) / derivative(x);

  while (fabs(delta_x) >= tolerance) {
    delta_x = equation(x) / derivative(x);
    x = x - delta_x;
  }
  return x;
};
```
File: _newtonraphson.cpp_.

From this definition, ``NewtonRaphson`` instances need to be initialized with a value for ``tolerance_in``, which is then
stored as the private member ``tolerance``. Once the object instance has been constructed, users can call its ``solve``
method to iteratively find ``equation``'s root, with ``equation`` and its ``derivative`` being imported from
``algebra.cpp`` via the ``include`` line near the top.

### Binding

The binding of the C++ code:

```cpp
#include <emscripten/bind.h>
#include "newtonraphson.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(newtonraphson) {
   class_<NewtonRaphson>("NewtonRaphson")
      .constructor<double>()
      .function("solve", &NewtonRaphson::solve)
      ;
}
```
File: _bindings.cpp_.

Here we expose the NewtonRaphson class by registering itself and its public members using [embind binding statements](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#classes).

### Compiling to WebAssembly

The Newton-Raphson source and its binding can be compiled into a WebAssembly module with Emscripten's ``emcc`` compiler, as follows:

```shell
emcc \
  -I. \
  -o newtonraphson.js \
  -Oz \
  -s MODULARIZE=1 \
  -s EXPORT_NAME=createModule \
  --bind newtonraphson.cpp bindings.cpp
```

This will generate a WebAssembly module ``newtonraphson.wasm``, along with a JavaScript file ``newtonraphson.js``. Using
this JavaScript library, we can find the root of the mathematical function, and subsequently display its value with the
following HTML:

```html
<html>
   <head>
      <!-- Load WebAssemlby module -->
      <script type="text/javascript" src="newtonraphson.js"></script>
   </head>
   <body>
      <div>
         Function root is approximately at x =
         <span id="answer"/>
      </div>
      <script>
         // Wait for module to initialize,
         createModule().then(({NewtonRaphson}) => {
            // Hardcoded input values
            const tolerance = 0.001;
            const initial_guess = -20;
            // Perform computation
            const newtonraphson = new NewtonRaphson(tolerance);
            const root = newtonraphson.solve(initial_guess);
            // Write root to tag with answer as identifier
            document.getElementById("answer").innerHTML = root.toFixed(2);
         });
      </script>
   </body>
</html>
```
File: _index.html_.

### Hosting the app with a web server

In order to display the HTML page in a web browser we will need a web server because the embedded JavaScript file can only be loaded in this way. We will use Python [http.server](https://docs.python.org/3/library/http.server.html) module for this.
hosted by a web server. Python3 ships with a built-in web server ``http.server``, we will use it to host all files on
port 8000.

```shell
python3 -m http.server 8000
```

Visit [http://localhost:8000/](http://localhost:8000/) to see the result of the calculation.

![result.png](result.png)

_The resulting page if everything works._

**TODO** (Recap and announce what else is coming)

The result of root finding was calculated using the C++ algorithm compiled to a WebAssembly module, executed by some
JavaScript and rendered on a HTML page.

The nice thing about this solution is that we don't need expensive infrastructure to perform computation as the
computation is done in the users web browser. We just need somewhere to host the files.

<!--
In upcoming blogs will see if we can perform the computation without blocking the user interface and make a nice
interactive form. In even more blogs we will look into performing the computation on the server with JavaScript and
Python in a human readable and compute readable format.
-->

If you enjoyed this article, make sure to give us a clap!
