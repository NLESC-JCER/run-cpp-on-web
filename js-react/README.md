# Fancy JS web application js-react.md
 webassembly web worker + react + simple html form

In the third part of the blog series. In the previous blog posts, we compiled the C++ algorithm into a webassembly code.
In this blog post we will create a web application using [React](https://reactjs.org/) and add auto-generated web-form which will generate a plot to display the result.

If you haven't read the first two parts please have a look at:

-[Part 1 Title]('link-to-part-1')
-[Part 2 Title]('link-to-part-2')

## React web application

The web application will wrote in [Part 1 Title]('link-to-part-1') needs to update the entire page to display the results. Even for small changes in the webpage this has to happen. Thanks to the advance web-browsers and Javascript, Single Page Applications (SPA) can update only required elements in the webpage.

With the advances in the web browser (JavaScript) engines including methods to fetch JSON documents from a web service, it has become possible to address this shortcoming. The so-called Single Page Applications (SPA) enable changes to be made in a part of the page without rendering the entire page. To ease SPA development, a number of frameworks have been developed. The most popular front-end web frameworks are (as of June 2020):

When the calculation form is submitted in the React application a web worker loads the wasm file, starts the calculation, renders the result. With this architecture the application only needs cheap static file hosting to host the HTML, js and wasm files. The calculation will be done in the web browser on the end users machine instead of a server.

To render the **React** application we need a HTML element as a container. We will give it the identifier container which will use later when we implement the **React** application in the app.js file.

```js
<!doctype html>
<html lang="en">
  <head>
    <title>Example React application</title>
    <<imports>>
  </head>
  <div id="container"></div>
  <script type="text/babel" src="app.js"></script>
</html>
```

```html
<script src="https://unpkg.com/react@16/umd/react.development.js" crossorigin></script>
<script src="https://unpkg.com/react-dom@16/umd/react-dom.development.js" crossorigin></script>
```
