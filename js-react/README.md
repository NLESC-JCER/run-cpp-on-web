# Run your C++ code on the web: interactive form with React

In the previous blog posts we compiled the C++ algorithm into a webassembly code, added a web worker to unblock the ui when running long running tasks. In this blog post we will create a web application using [React](https://reactjs.org/). The web application will have a web-form which allows us to change the parameters of the algorithm.

If you haven't read the first two parts please have a look at:

-[Part 1: web assembly](https://github.com/NLESC-JCER/run-cpp-on-web/blob/master/js-webapp)

-[Part 2: unblock ui with web worker](https://github.com/NLESC-JCER/run-cpp-on-web/blob/master/js-webapp-async)

## React web application

The web application we developed so far in needs to update the entire page to display the results. Even for small changes in the webpage this has to happen. Thanks to the advance web-browsers and JavaScript, Single Page Applications (SPA) can update only required elements in the webpage. We will use one of the most popular web-frameworks, React, to develop the SPA.

The form in the web application will collect the user inputs and use them to initialize the algorithm(**add link?**). When the form is submitted, a web worker loads the wasm file, starts the calculation, renders the result. With this architecture the application only needs cheap static file hosting to host the HTML, JavaScript and WebAssembly files. The algorithm will be running in the web browser on the end users machine instead of a server.

### The Html code

To render the React application we need a HTML element as a container. We will give it the identifier **container** which will use later when we implement the **React** application in the [javascript section](js-section).

We will keep the html code very minimal. The code will contain three essential elements:

- **\<head\>** element to set the title and to load the required external dependencies (javascript libraries).

  ```html
  <head>
    <title>Example React application</title>
    <script src="https://unpkg.com/react@16/umd/react.development.js" crossorigin></script>
    <script src="https://unpkg.com/react-dom@16/umd/react-dom.development.js" crossorigin></script>
    <script src="https://unpkg.com/babel-standalone@6/babel.min.js"></script>
  </head>
  ```

- **\<div\>** element to discplay the result

  ```html
  <div id="container"></div>
  ```

- **\<script\>** element to load the Javascript application

  ```html
    <script type="text/babel" src="app.js"></script>
  ```

The complete html code will look like this:

```html
<!doctype html>
<html lang="en">
  <head>
    <title>Example React application</title>
    <script src="https://unpkg.com/react@16/umd/react.development.js" crossorigin></script>
    <script src="https://unpkg.com/react-dom@16/umd/react-dom.development.js" crossorigin></script>
    <script src="https://unpkg.com/babel-standalone@6/babel.min.js"></script>
  </head>
  <div id="container"></div>
  <script type="text/babel" src="app.js"></script>
</html>
```

## Javascript code (React)

Similarly we will split the JavasSript code into sections and build up the the React application from React components.

Let's start with the header part. We will define a JavaScript function which returns the header element which will be rendered by the web-browser when the user visits the page.

```js
// this JavaScript snippet is later referred to as <<heading-component>>
function Heading() {
  const title = 'Root finding web application';
  return <h1>{title}</h1>
}
```

The return statement of this function looks weird, right? It is indeed not HTML. React uses a syntax extension called [JSX](https://reactjs.org/docs/introducing-jsx.html) to describe the UI. With the magical conversion of [Babel](https://babeljs.io/docs/en/next/babel-standalone.html) we can convert JSX into JavaScript code. After this conversion the generated JavaScript code will look like:

```js
function Heading() {
  const title = 'Root finding web application';
  return React.createElement('h1', null, `{title}`);
}
```

I now can hear what you are saying: but wait... How do I use Babel? We haven't inclueded it anywhere. Yes, we did. Babel was already added to the HTML code.

```html
<script src="https://unpkg.com/babel-standalone@6/babel.min.js"></script>
```

In order the header element to be rendered we need to tell **ReactDOM** which element it should render and where it should be displayed. Do you still remember the **container** div we defined in the HTML part?

```js
// this JavaScript snippet appenended to react/app.js
ReactDOM.render(
  <Heading/>,
  document.getElementById('container')
);
```

The complete code should like this.

```js
function Heading() {
  const title = 'Root finding web application';
  return <h1>{title}</h1>
}

ReactDOM.render(
  <Heading/>,
  document.getElementById('container')
);
```

When the page is rendered, the generated HTML code will be like:

```html
<h1>Root finding web application</h1>;
```

## Adding the  web form

The web application in our example should have a form with tolerance and initial_guess input fields, as well as a submit button. The form in JSX can be written in the following way:

```js
<form onSubmit={handleSubmit}>
  <label>
    Epsilon:
    <input name="tolerance" type="number" value={tolerance} onChange={onToleranceChange}/>
  </label>
  <label>
    Initial guess:
    <input name="initial_guess" type="number" value={initial_guess} onChange={onGuessChange}/>
  </label>
  <input type="submit" value="Submit" />
</form>
```


## Extra notes

The code supplied here should not be used in production as converting JSX in the web browser is slow. It's better to use [Create React App](http://create-react-app.dev/) which gives you an infrastructure to perform the transformation offline.
