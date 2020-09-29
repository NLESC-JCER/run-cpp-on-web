Repository for draft blog post based on [cpp2wasm guide](https://github.com/NLESC-JCER/cpp2wasm).
Blogs will be published at [https://medium.com/@eScienceCenter](https://medium.com/@eScienceCenter)

- audience: RSEs
- approach: text before code, include code if it supports the story
- each blog has a focus and will try to minimize distractions, for example React blog should not use web worker and plot blog should use straight vegalite not react.
- each blog has its own code, will make sure all they expand the first blog

The blog topics are

1. [Using C++ in a web app with WebAssembly](webassembly/README.md)
1. [Help! My C++ web app is not responding](web-worker/README.md)
1. [Interact with your C++ web app using React forms](react/README.md)
1. [Spice up your C++ web app with visualizations](vega/README.md)
1. [C++ web app with WebAssembly, Vega, Web Worker and React](kitchen-sink/README.md)

Blog ideas:

1. when stuff goes wrong, handling exceptions from C++
1. computation takes too long for browser, use a web service running on a server with Fastify
1. computation blocks my service, use thread pool
1. My head can't process React, show the form using Vue.js
1. I don't wanna learn a JS framework, give me a form with vanilla js
1. I don't like JavaScript, give me Python, use pybind11 to interact with C++ from Python
1. Expose C++ on the web using Python, use a web service running on a server with connexion
1. is it done yet?, give progress during calculation, use Celery (Python)
1. My C++ program needs sockets, can I use those? Explain limitations of Emscripten, use [web socket](https://emscripten.org/docs/porting/networking.html) and in-memory filesystem as examples
1. How to build WebAssembly with a dependencies like boost, explain [bundled standard libs](https://emscripten.org/docs/compiling/Building-Projects.html#using-libraries) and https://github.com/emscripten-ports
1. Using a web worker and WebAssembly I would like to know how to report progress of the calculation back to the user. Also my computation should be stopped when the user submits again or presses a cancel button.
1. Current code does not throw exceptions, but may be usefull to explain how to convert error number to message.
See https://emscripten.org/docs/optimizing/Optimizing-Code.html#optimizing-code-exception-catching
