# C++ in a web browser

I am a research software engineer working at the Netherlands eScience center working together with a researcher who wrote a algorithm in the C++ language.
To make the algorithm more usable by others I want make it accessable from a web browser.
Let's go through the steps to create a web application in this blog and upcoming blogs as a reminder for myself and for others in a similar situation.

The algorithm of the researcher is a bit to complex to talk about here so I will use the [Newton-Rapson root finding algorithm](https://en.wikipedia.org/wiki/Newton%27s_method) as a replacement. The C++ implementation of root finding algorithm will use a class with an epsilon value to initialize and a solve function which accepts an initial guess and returns the found root value. The C+++ source code can be found [here](https://github.com/NLESC-JCER/run-cpp-on-web/tree/master/js-webapp).

You can visit a web site with C++ source code using a web browser, but it can not run the code for you. If so that would make this a very short story. So we need some way to make the web browser able to run the code. Luckily all web browsers can run a programming language called [JavaScript](https://developer.mozilla.org/en-US/docs/Web/javascript), let us see if we can use that. Tools where made in the past that can transpile non-JavaScript code to JavaScript, but the performance was less than running native code. To run code as fast as native code, the [WebAssembly language](https://webassembly.org/) was developed. WebAssembly is a low-level, [Assembly](https://en.wikipedia.org/wiki/Assembly_language)-like language with a compact binary format. The binary format is stored as a WebAssembly module or *.wasm file. The JavaScript engine inside web browsers can load WebAssembly modules and interact with them.

Instead of writing code in the WebAssembly language, there are compilers that can take C++/C code and compile it to a WebAssembly module. [Emscripten](https://emscripten.org/) is the most popular C++ to WebAssembly compiler. Emscripten has been successfully used to port game engines like the Unreal engine to the browser making it possible to have complex 3D games in the browser without needing to install anything else than the web browser. So we found a solution we can use Emscripten to compile our C++ code to WebAssembly which can then be loaded by a web browser and interacted with using JavaScript.

To use Emscripten we need to install it by following the [installation instructions](https://emscripten.org/docs/getting_started/downloads.html).
To call C++ code (which has been compiled to a WebAssembly module) from JavaScript, a binding is required. The binding will map C++ constructs to their JavaScript equivalent and back. The binding called [embind](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind) is declared in a C++ file which is included in the compilation.

The binding of the C++ code we want to call looks like:

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

The algorithm and binding can be compiled into a WebAssembly module with the Emscripten compiler called [emcc](https://emscripten.org/docs/tools_reference/emcc.html).

The command to compile is

```shell
emcc -I. -o newtonraphsonwasm.js \
  -Oz -s MODULARIZE=1 -s EXPORT_NAME=createModule \
  --bind newtonraphson.cpp wasm-newtonraphson.cpp
```

The compiler generated a JavaScript file which we can embed in a HTML page.
We need to wait for the createModule to download and initialize the WebAssembly module then we can start calling functions we defined in the binding. The last step is to render the answer on the page with a [document manipulation method](https://developer.mozilla.org/en-US/docs/Web/API/Document/getElementById).

```html
<!doctype html>
<html lang="en">
  <head>
    <title>Example</title>
    <script type="text/javascript" src="newtonraphsonwasm.js"></script>
    <script>
      const module = await createModule();
      const epsilon = 0.001;
      const finder = new module.NewtonRaphson(epsilon);
      const guess = -20;
      const root = finder.solve(guess);
      document.getElementById('answer').innerHTML = root.toFixed(2);
    </script>
  </head>
  <body>
    <span id="answer"> </span>
  </body>
</html>
```

Let save the HTML page above in `example.html` file.

We can not just open this html page in a web browser as the embedded JavaScript file can only be loaded when it is hosted by a web server. Python ships with a built-in web server, we will use it to host all files on port 8000.

```shell
python3 -m http.server 8000
```

Visit [http://localhost:8000/example.html](http://localhost:8000/example.html) to see the result of the calculation. Embedded below is the example hosted on [GitHub pages](https://nlesc-jcer.github.io/run-cpp-on-web/js-webapp/example.html).

<iframe width="100%" height="60" src="https://nlesc-jcer.github.io/run-cpp-on-web/js-webapp/example.html" /></iframe>

When everything went OK we should see a page with `-1.00` which is the correct answer for the root of the defined equation.

The result of root finding was calculated using the C++ algorithm compiled to a WebAssembly module, executed by some JavaScript and rendered on a HTML page.

The nice thing about this solution is that we don't need expensive infrastructure to perform computation as the computation is done in the users web browser. We just need somewhere to host the files.

In upcoming blogs will see if we can perform the computation without blocking the user interface and make a nice interactive form. In even more blogs we will look into performing the computation on the server with JavaScript and Python in a human readable and compute readble format.
