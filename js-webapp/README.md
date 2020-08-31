# Run your C++ code on the web

Let's say you have some C or C++ code laying around that you would like to make available to a wider audience, by putting
it on the web. Until recently, this used to be pretty difficult, and may even have required reimplementation of the
software in JavaScript, the programming language that browsers use.

_Wouldn't it be great if you could run your existing C/C++ code on the web with only minor effort?_

That way, loads more people would be able to see your results, interact with your algorithm, and apply it for their own
purposes.

In this blog, we'll show you how to take a simple algorithm written in C++ and make it available as a simple web
application. Subsequent blogs in this series will expand on the current one by laying out more advanced topics,
specifically how to [make the app interactive](anotherblog), how to [visualize the results](someotherblog), and
how to [deal with long running tasks](yetanotherblog).

So today's aim is to have a simple web app like the code snippet below. At first glance this is just some boilerplate to
be able to use the ``newtonraphson.js`` library, but **the neat part is that the complete Newton-Raphson code is written
in C++, not JavaScript**. With some trickery, we'll be able to use that C++ code from the browser, without the need to
port it first!

```html
<!doctype html>
<html lang="en">
   <head>
      <title>Newton-Raphson</title>
      <meta charset="utf-8">
      <script type="text/javascript" src="newtonraphson.js"></script>
      <script>
         createModule().then((module) => {
            const tolerance = 0.001;
            const newtonraphson = new module.NewtonRaphson(tolerance);
            const initial_guess = -20;
            const root = newtonraphson.solve(initial_guess);
            document.getElementById("answer")
               .innerHTML = "Function root is approximately at x = " +
                            root.toFixed(2);
         });
      </script>
   </head>
   <body>
      <span id="answer"> </span>
   </body>
</html>
```

Now before you say _"That'll be so much slower than running it native!"_ or _"C/C++ from the browser? Impossible!"_,
just hold your horses for a sec. With the right tools, it is possible to run C/C++ code in the browser, without any
significant performance penalty. (Mention WebAssembly). Using this approach, (these peeps) were able to run the video
game (X) in the browser and didn't find any performance problems. And if it works for video games, it will likely work
for your research software, too. 

![hold-your-horses.jpeg](hold-your-horses.jpeg)

_Hold your horses._

## What we'll need

OK, now that you're fully on board with this, let's get to it. Here's a list of what we need:

1. Some C/C++ code to illustrate the process. For this, we'll use some C++ code that implements the _Newton-Raphson_
root finding method (You remember Newton? Quiet fellow, fabulous hair? Yes, him). Newton-Raphson is a so-called _root
finding_ algorithm, i.e. a method to find the value of _x_ where it crosses _y_ for a given mathematical function.
1. A program that can take our existing C/C++ code and compile it into a WebAssembly module. Modern browsers are able to
run WebAssembly without loss of performance. For this, we'll use [Emscripten](https://emscripten.org/)'s ``emcc``
compiler, the most popular C++ to WebAssembly compiler of the bunch.
1. To use the WebAssembly functionality from JavaScript, a binding is required. The binding will map C++ constructs to
their JavaScript equivalent and back. For this, we'll use 
[embind](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind).
1. A web server to serve our files. We'll use Python 3's ``http.server``, but other options like X and Y work equally well.


## Tying it all together

<!--
Basically, take the C++ and the bindings, then use emcc to compile it into wasm, then call the result from JS. Show the required code snippets
-->

### The C++ code

Here is the equation whose root we want to find, along with its derivative, since that's what Newton-Raphson requires:

```cpp
namespace algebra {

   // An example equation
   double equation(double x) {
      return x * x * x - x * x + 2;
   }

   // Derivative of the above equation
   double derivative(double x) {
      return 3 * x * x - 2 * x;
   }
}
```
File: _algebra.cpp_

The header file for the Newton-Raphson iterative root finding algorithm:

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

File ``newtonraphson.hpp`` defines a class ``NewtonRaphson`` in namespace ``rootfinding``. Besides the constructor
method ``NewtonRaphson(double tolerancein)``, ``NewtonRaphson`` has one other public method, ``solve``, which takes a
``double``, and returns another ``double``. Furthermore, ``NewtonRaphson`` also has a private member, ``tolerance`` of
type ``double``, which is used to store the class instance's private data.

File ``newtonraphson.cpp`` contains the implementation corresponding to the header file's definition:

```cpp
#include "newtonraphson.hpp"
#include "algebra.cpp"
#include <math.h>

using namespace algebra;

namespace rootfinding {

   // Define the constructor method of NewtonRaphson instances
   NewtonRaphson::NewtonRaphson(double tolerancein) : tolerance(tolerancein) {}

   // Define the 'solve' method of NewtonRaphson instances
   double NewtonRaphson::solve(double xin) {
      double x = xin;
      double delta_x = equation(x) / derivative(x);

      while (fabs(delta_x) >= tolerance) {
         delta_x = equation(x) / derivative(x);
         x = x - delta_x;
      }
      return x;
   };
}
```
File: _newtonraphson.cpp_

From this definition, ``NewtonRaphson`` instances need to be initialized with a value for ``tolerancein``, which is then
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
   class_<rootfinding::NewtonRaphson>("NewtonRaphson")
      .constructor<double>()
      .function("solve", &rootfinding::NewtonRaphson::solve)
      ;
}
```
File: _bindings.cpp_

(Explain the above snippet in functional terms)

### Compiling to WebAssembly

The Newton-Raphson source and its binding can be compiled into a WebAssembly module with Emscripten's ``emcc`` compiler, as follows:

```shell
emcc -I. -o newtonraphson.js \
  -Oz -s MODULARIZE=1 -s EXPORT_NAME=createModule \
  --bind newtonraphson.cpp bindings.cpp
```

This will generate a WebAssembly module ``newtonraphson.wasm``, along with a JavaScript file ``newtonraphson.js``. Using
this JavaScript library, we can find the root of the mathematical function, and subsequently display its value with the
following HTML:

```html
<!doctype html>
<html lang="en">
   <head>
      <title>Newton-Raphson</title>
      <meta charset="utf-8">
      <script type="text/javascript" src="newtonraphson.js"></script>
      <script>
         createModule().then((module) => {
            const tolerance = 0.001;
            const newtonraphson = new module.NewtonRaphson(tolerance);
            const initial_guess = -20;
            const root = newtonraphson.solve(initial_guess);
            document.getElementById("answer")
               .innerHTML = "Function root is approximately at x = " +
                            root.toFixed(2);
         });
      </script>
   </head>
   <body>
      <span id="answer"> </span>
   </body>
</html>
```
_example.html_

When this page is loaded, ... (explain what's happening)
The last step is to render the answer on the page using the document manipulation method
[``getElementById``](https://developer.mozilla.org/en-US/docs/Web/API/Document/getElementById).

### Hosting the app with a web server

We can not just open this HTML page in a web browser, as the embedded JavaScript file can only be loaded when it is
hosted by a web server. Python3 ships with a built-in web server ``http.server``, we will use it to host all files on
port 8000.

```shell
python3 -m http.server 8000
```

Visit [http://localhost:8000/](http://localhost:8000/) to see the result of the calculation. 

![result.png](result.png)

Caption _When everything went OK we should see a page with `-1.00` which is the correct answer for the root of the
defined equation given the initial guess of -20._

(Recap and announce what else is coming)

The result of root finding was calculated using the C++ algorithm compiled to a WebAssembly module, executed by some
JavaScript and rendered on a HTML page.

The nice thing about this solution is that we don't need expensive infrastructure to perform computation as the
computation is done in the users web browser. We just need somewhere to host the files.

In upcoming blogs will see if we can perform the computation without blocking the user interface and make a nice
interactive form. In even more blogs we will look into performing the computation on the server with JavaScript and
Python in a human readable and compute readable format.

If you enjoyed this article, make sure to give us clap!
