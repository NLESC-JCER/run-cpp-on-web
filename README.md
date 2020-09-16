[Rendered version](https://nlesc-jcer.github.io/run-cpp-on-web/)

- audience: RSEs
- approach: text before code, include code if it supports the story
- each blog has a focus and will try to minimize distractions, for example React blog should not use web worker and plot blog should use straight vegalite not react.
- each blog has its own code, will make sure all they expand the first blog

The blog topics are

1. web assembly, [small static web page without user interaction](js-webapp)
1. unblock ui with web worker, [how to deal with tasks that take longer to complete](js-webapp-async)
1. interactive form with React, [gathering user inputs through a form in a simple react app](js-react)
1. plot with Vega, [displaying results from the algorithm in a more complex web app](js-plot)
1. blog which combines web-worker, form with React and plot with vega into single example

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
