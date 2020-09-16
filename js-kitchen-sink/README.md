# Run C++ on the web with web assembly, web workers, React and Vega

In previous blogs in the series we learned about

* compile C++ to web assembly
* do not block main thread with web workers
* interactive form with react
* plotting with vega-lite

Those pieces by them selves are useful, but how can all those lessons by combined into a single web application. This blog will tell you how to do build the web application including everyhing but the kitchen sink.

![Pack everything but the kitchen sink](1024px-Pack_Gong_(3144438149).jpg)

We want to make an React application which will find the root of an equation using Newton-Raphson algorithm and plot each iteration of the algorithm. Let us go over the pieces for this application next.

## 1. WebAssembly module

The WebAssembly module contains the equation and Newton-Raphson algorithm. We will reuse the module made in the visualization blog post, namely [newtonraphson.js](https://github.com/NLESC-JCER/run-cpp-on-web/blob/master/js-plot/newtonraphson.js) and [newtonraphson.wasm](https://github.com/NLESC-JCER/run-cpp-on-web/blob/master/js-plot/newtonraphson.wasm)

## 3. Web Worker

In the [web worker blog](TODO) we used a Web Worker thread to not block the user interface while computation. The application we are going to make here we want to use that as well.

The work code of the Web Worker blog needs to be enhanced with returning the iterations like

```javascript
// this JavaScript snippet is stored as webassembly/worker.js
importScripts('newtonraphson.js');

// this JavaScript snippet is later referred to as <<worker-provider-onmessage>>
onmessage = function(message) {
  // this JavaScript snippet is before referred to as <<handle-message>>
  if (message.data.type === 'CALCULATE') {
    createModule().then(({NewtonRaphson}) => {
      // this JavaScript snippet is before referred to as <<perform-calc-in-worker>>
      const tolerance = message.data.payload.tolerance;
      const newtonraphson = new NewtonRaphson(tolerance);
      const initial_guess = message.data.payload.initial_guess;
      const root = newtonraphson.solve(initial_guess);
      // newtonraphson.iterations is a vector object which not consumeable by Vega
      // So convert Emscripten vector of objects to JavaScript array of objects
      const iterations = new Array(
        newtonraphson.iterations.size()
      ).fill().map(
        (_, iteration) => {
          return newtonraphson.iterations.get(iteration)
        }
      );
      // this JavaScript snippet is before referred to as <<post-result>>
      postMessage({
        type: 'RESULT',
        payload: {
          root: root,
          iterations: iterations
        }
      });
    });
  }
};

```
File: _worker.js_

## 3. React

In a previous blog we wrote several React components to make a form let's re-use most of it, but instead of calling the WebAssembly module directly we will use a Web Worker.

```js
const [result, setResult] = React.useState({ root: undefined, iterations: [] });

function handleSubmit(event) {
    event.preventDefault();
    const worker = new Worker('worker.js');
    worker.onmessage = function (message) {
        if (message.data.type === 'RESULT') {
        const result = message.data.payload;
        setResult(result);
        worker.terminate();
        }
    };
    worker.postMessage({
        type: 'CALCULATE',
        payload: { tolerance: tolerance, initial_guess: initial_guess }
    });
}
```

## 4. Visualization

In the previous blog post about visualization using Vega-Lite we made 2 plots. Let's combine the 2 plots into a single visualization where zooming and panning in one plot will be reflected in other plot.

To generate the Vega-Lite specification we can write a function like so

```js
function iterations2spec(iterations) {
    // Because the initial guess can be changed now we need to scale equation line to iterations
    const max_x = Math.max(...iterations.map(d => d.x));
    const min_x = Math.min(...iterations.map(d => d.x));
    const equation_line = {
      "width": 800,
      "height": 600,
      "data": {
        "sequence": {
          // Draw equation within -4 to 4 window, if iterations are inside of window
          // otherwise use iterations min to max x
          "start": min_x > -4 ? -4 : min_x,
          "stop": max_x < 4 ? 4 : max_x,
          "step": 0.1, "as": "x"
        }
      },
      "transform": [
        {
          "calculate": "2 * datum.x * datum.x * datum.x - 4 * datum.x * datum.x + 6",
          "as": "y"
        }
      ],
      "mark": "line",
      "encoding": {
        "x": { "type": "quantitative", "field": "x" },
        "y": { "field": "y", "type": "quantitative" },
        "color": { "value": "#f00" }
      }
    };
    const root_rule = {
      "data": { "values": [{ "x": -1 }] },
      "encoding": { "x": { "field": "x", "type": "quantitative" } },
      "layer": [
        { "mark": { "type": "rule", "strokeDash": [4, 8] } },
        { "mark": { "type": "text", "align": "left", "dx": 4, "text": "Root" } }
      ]
    };
    const iterations_scatter = {
      "title": "2x^3 - 4x^2 + 6 with root",
      "data": {
        "values": iterations
      },
      "encoding": {
        "x": { "field": "x", "type": "quantitative" },
        "y": { "field": "y", "type": "quantitative" },
        "text": { "field": "index", "type": "nominal" }
      },
      "layer": [
        {
          "mark": { "type": "circle", "tooltip": { "content": "data" } },
          "selection": { "grid": { "type": "interval", "bind": "scales" } }
        },
        { "mark": { "type": "text", "dy": -10 } }
      ]
    };
    const iteration_vs_y = {
      "title": "Iterations",
      "width": 400,
      "height": 600,
      "data": {
        "values": iterations
      },
      "encoding": {
        "x": { "field": "index", "type": "ordinal", "title": "Iteration index" },
        "y": { "field": "y", "type": "quantitative" }
      },
      "mark": {
        "type": "line",
        "point": true,
        // Enable tooltip so on mouseover it shows all data of that iteration
        "tooltip": { "content": "data" }
      },
      // Enable zooming and panning
      "selection": { "grid": { "type": "interval", "bind": "scales" } }
    };

    return {
      "$schema": "https://vega.github.io/schema/vega-lite/v4.json",
      "hconcat": [
        {
          "layer": [
            equation_line,
            root_rule,
            iterations_scatter
          ]
        }
        , iteration_vs_y
      ]
    };
}
```

TODO show the code

TODO show usage via animated gif

TODO link to live demo


We'll need a web server to display the HTML page in a web browser. For this, we'll use the http.server module from Python 3 to host all files on port 8000, like so:

```shell
python3 -m http.server 8000
```

Visiting the page should give us a plot like

[![Image](composite.png)](https://nlesc-jcer.github.io/run-cpp-on-web/js-kitchen-sink/index.html)
(Click on image to get interactive version)

TODO outro,

TODO link to GitHub repo and https://nlesc-jcer.github.io/cpp2wasm ?
