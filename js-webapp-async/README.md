- audience: RSEs
- approach: text before code, include code if it supports the story
- live demos: 

# Introduction
In a [previous blogpost](../js-webapp/README.md) we discussed how to run c++ code on the web using Javascript.

We created a webapp that executed some C++ code and showed the result, which was a single number. While the page was running the C++ code, the page was blocked and unresponsive. That was not a problem then, because the computation done in the code was tiny. This becomes a problem when we are performing long running tasks instead. How to prevent blocking when running long running tasks in c++?

In this blogpost, we will use web workers to solve this problem by running the code asynchronously.

### Long-running tasks with web worker

Executing a long running C++ method will block the browser from running any other code like updating the user interface.
In order to avoid this, the method can be run in the background using [web
workers](https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Using_web_workers). A web worker runs in its
own thread and can be interacted with from JavaScript using messages.

We need to instantiate a web worker which we will implement later in `webassembly/worker.js`.

```{.js #worker-consumer}
// this JavaScript snippet is later referred to as <<worker-consumer>>
const worker = new Worker('worker.js');
```

We need to send the worker a message with description for the work it should do.

```{.js #worker-consumer}
// this JavaScript snippet is appended to <<worker-consumer>>
worker.postMessage({
  type: 'CALCULATE',
  payload: { epsilon: 0.001, guess: -20 }
});
```

In the web worker we need to listen for incoming messages.

```{.js #worker-provider-onmessage}
// this JavaScript snippet is later referred to as <<worker-provider-onmessage>>
onmessage = function(message) {
  <<handle-message>>
};
```

Before we can handle the message we need to import the WebAssembly module.

```{.js file=webassembly/worker.js}
// this JavaScript snippet is stored as webassembly/worker.js
importScripts('newtonraphsonwasm.js');

<<worker-provider-onmessage>>
```

We can handle the `CALCULATE` message only after the WebAssembly module is loaded and initialized.

```{.js #handle-message}
// this JavaScript snippet is before referred to as <<handle-message>>
if (message.data.type === 'CALCULATE') {
  createModule().then((module) => {
    <<perform-calc-in-worker>>
    <<post-result>>
  });
}
```

Let's calculate the result (root) based on the payload parameters in the incoming message.

```{.js #perform-calc-in-worker}
// this JavaScript snippet is before referred to as <<perform-calc-in-worker>>
const epsilon = message.data.payload.epsilon;
const finder = new module.NewtonRaphson(epsilon);
const guess = message.data.payload.guess;
const root = finder.solve(guess);
```

And send the result back to the web worker consumer as a outgoing message.

```{.js #post-result}
// this JavaScript snippet is before referred to as <<post-result>>
postMessage({
  type: 'RESULT',
  payload: {
    root: root
  }
});
```

Listen for messages from worker and when a result message is received put the result in the HTML page like we did before.

```{.js #worker-consumer}
// this JavaScript snippet is appended to <<worker-consumer>>
worker.onmessage = function(message) {
  if (message.data.type === 'RESULT') {
    const root = message.data.payload.root;
    <<render-answer>>
  }
}
```

Like before we need a HTML page to run the JavaScript, but now we don't need to import the `newtonraphsonwasm.js` file
here as it is imported in the `worker.js` file.

```{.html file=webassembly/example-web-worker.html}
<!doctype html>
<!-- this HTML page is stored as webassembly/example-web-worker.html -->
<html lang="en">
  <head>
    <title>Example web worker</title>
    <script>
      <<worker-consumer>>
    </script>
  </head>
  <body>
    <span id="answer"> </span>
  </body>
</html>
```

Like before we also need to host the files in a web server with

```shell
python3 -m http.server 8000
```

Visit
[http://localhost:8000/webassembly/example-web-worker.html](http://localhost:8000/webassembly/example-web-worker.html)
to see the result of the calculation. Embedded below is the example hosted on [GitHub
pages](https://nlesc-jcer.github.io/cpp2wasm/webassembly/example-web-worker.html)

<iframe width="100%" height="60" src="https://nlesc-jcer.github.io/cpp2wasm/webassembly/example-web-worker.html" /></iframe>

The result of root finding was calculated using the C++ algorithm compiled to a WebAssembly module, imported in a web
worker (separate thread), executed by JavaScript with messages to/from the web worker and rendered on a HTML page.

# Outro
In this blogpost we have learned how to run, computationally intensive c++ code on the web using web workers.

In the [next blogpost](../js-react/README.md) we will show how to expand the code to an interactive React webapp.
