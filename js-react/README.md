#  Fancy JS web application js-react.md
<!-- react, json-schema form, vega plot -->

In the third and final part of the blog series. In the previous blog posts, we compiled the C++ algorithm into a webassembly code.
In this blog post we will create a web application using [React](https://reactjs.org/) and add auto-generated web-form which will generate a plot to display the result.

If you haven't read the firt two parts please have a look at:

-[Part 1]('')
-[Part 2]('')

## React web application

When the calculation form is submitted in the React application a web worker loads the wasm file, starts the calculation, renders the result. With this architecture the application only needs cheap static file hosting to host the HTML, js and wasm files. The calculation will be done in the web browser on the end users machine instead of a server.

To render the **React** application we need a HTML element as a container. We will give it the identifier container which will use later when we implement the **React** application in the [app.js](react/app.js) file.

## Auto-generated web-form using json-schema


## Displaying results with Vega.js
