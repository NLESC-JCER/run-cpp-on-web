importScripts('newtonraphson.js');

onmessage = function(message) {
  if (message.data.type === 'CALCULATE') {
    createModule().then((module) => {
      const epsilon = message.data.payload.epsilon;
      const finder = new module.NewtonRaphson(epsilon);
      const guess = message.data.payload.guess;
      const root = finder.solve(guess);
      postMessage({
        type: 'RESULT',
        payload: {
          root: root
        }
      });
    });
  }
};
