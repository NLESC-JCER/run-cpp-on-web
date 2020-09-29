importScripts('newtonraphson.js');

onmessage = function(message) {
  if (message.data.type === 'CALCULATE') {
    createModule().then(({NewtonRaphson}) => {
      const tolerance = message.data.payload.tolerance;
      const finder = new NewtonRaphson(tolerance);
      const initial_guess = message.data.payload.initial_guess;
      const root = finder.solve(initial_guess);
      postMessage({
        type: 'RESULT',
        payload: {
          root: root
        }
      });
    });
  }
};
