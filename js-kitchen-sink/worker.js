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
