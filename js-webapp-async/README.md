# Introduction
In a [previous blogpost](../js-webapp/README.md) we discussed how to run c++ code on the web using Javascript.

We created a webapp that executed some C++ code and showed the result. While the page was running the C++ code, the page was blocked and unresponsive. That was not a problem then, because the computation done in the code was tiny. This becomes a problem when we are performing long running tasks instead. How to prevent blocking when running long running tasks in c++?

In this blogpost, we will use web workers to solve this problem by running the code asynchronously.

### Long-running tasks with web worker

Let's have a look at the code we ended up with in last blog. When loading the page, the webassembly is excecuted, after which the page can finish rendering. Because the webassembly code was very quick, this was fine. For this blog we assume we have a more long running task. We create such task artificially by adding a few seconds of sleep in the c++ code (TODO show snippet). Like in the previous post, we compile the c++ code to create webassembly. The resulting page with our slow task is now [here](https://nlesc-jcer.github.io/run-cpp-on-web/js-webapp-async/example-blocking.html).

Notice that we also added a slider input to the page. This slider isn't connected to anything, and is there to illustrate UI blocking. Notice that while the webassembly code is still running, the slider is completely unresponsive. If this were an actual webapp and not just a demo, the UI blocking would surely annoy any users of the system and possibly make working with the app cumbersome and impracticle. We can easily solve this, and keep the UI responsive at all times, using web workers.

![blocking ui](blocking.gif)

Blocked ui while code is running

### Web workers

A [web worker](https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API) is an object that handles execution of a piece of code aynchronously. On completion the web worker will notify so the result can be processed and, in this case, rendered on the page. 


The way the page communicates with the worker object is through sending messages. The page will send a message to the worker to start doing work, and the message will include all data that the worder needs. The worker will perform its execution, using only the data that was in the message. The worker will not be able to access any data in the webapp directly. The worker can only send back results by sending a message back to the webapp. 

### The resulting page

Below is the code for the webapp. Notice the creation of the Worker object, the message that is posted and contains the data, and the instructions for handling any returning message that contains results. 

The webpage that uses the worker will look like:
```html
<!doctype html>
<html lang="en">
  <head>
    <title>Example web worker</title>
    <script>
      const worker = new Worker('worker.js');
      worker.postMessage({
        type: 'CALCULATE',
        payload: { epsilon: 0.001, guess: -20 }
      });
      worker.onmessage = function(message) {
        if (message.data.type === 'RESULT') {
          const root = message.data.payload.root;
          document.getElementById('answer')
		     .innerHTML = "Function root is approximately at x = " +
                            root.toFixed(2);
        }
      }
    </script>
  </head>
  <body>
<div class="slidecontainer">
  <input type="range" min="1" max="100" value="50" class="slider" id="myRange">
</div>
    <span id="answer"> </span>
  </body>
</html>
```

The worker code will contain only handling the incoming message. In this case, the worker will unpack the message, do the calculation, and pack the results in a new message that it will send back.
The code for the worker in worker.js will now look like:
```js
// this JavaScript snippet is stored as webassembly/worker.js
importScripts('newtonraphsonwasm.js');

// this JavaScript snippet is later referred to as <<worker-provider-onmessage>>
onmessage = function(message) {
  // this JavaScript snippet is before referred to as <<handle-message>>
  if (message.data.type === 'CALCULATE') {
    createModule().then((module) => {
      // this JavaScript snippet is before referred to as <<perform-calc-in-worker>>
      const epsilon = message.data.payload.epsilon;
      const finder = new module.NewtonRaphson(epsilon);
      const guess = message.data.payload.guess;
      const root = finder.solve(guess);
      // this JavaScript snippet is before referred to as <<post-result>>
      postMessage({
        type: 'RESULT',
        payload: {
          root: root
        }
      });
    });
  }
};
```

We can see the code in action [here](https://nlesc-jcer.github.io/run-cpp-on-web/js-webapp-async/example-web-worker.html). The calculation still takes the same time to perform, but as you will notice, the slider will be responsive all the time.

![responsive ui](web-worker.gif)

Responsive ui thanks to web-worker

# Outro
In this blogpost we have learned how to run, long running c++ code on the web using web workers.

In the [next blogpost](../js-react/README.md) we will show how to expand the code to an interactive React webapp.
