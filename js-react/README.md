# A Javascript web application using React

---
## Blog info (will be removed)

Topics:
- webassembly web worker
- react app
- simple html form

Covers the content from
- [react-application](https://nlesc-jcer.github.io/cpp2wasm/#/?id=react-application)

until

- [json-schema-powered-form](https://nlesc-jcer.github.io/cpp2wasm/#/?id=json-schema-powered-form)

---

In the previous blog posts we compiled the C++ algorithm into a webassembly code. In this blog post we will create a web application using [React](https://reactjs.org/). The web application will have a web-form to change the parameters of the algorithm and display the result.

If you haven't read the first two parts please have a look at:

-[Part 1 Title]('link-to-part-1')

-[Part 2 Title]('link-to-part-2')

## React web application

The web application we wrote in [Part 1 Title]('link-to-part-1') needs to update the entire page to display the results. Even for small changes in the webpage this has to happen. Thanks to the advance web-browsers and Javascript, Single Page Applications (SPA) can update only required elements in the webpage. We will use one of the most popular web-frameworks, React, to develop the SPA.

The form in the web application will collect the user inputs and use them to initialize the [algorithm]('link-to-algo') we use. When the form is submitted, a web worker loads the wasm file, starts the calculation, renders the result. With this architecture the application only needs cheap static file hosting to host the HTML, js and wasm files. The algorithm willbe running in the web browser on the end users machine instead of a server.

So let's start...

### Html code

To render the React application we need a HTML element as a container. We will give it the identifier **container** which will use later when we implement the **React** application in the [javascript section]('js-section').

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

Similarly we will split the Javascript code into sections and build up the the React application from React components.

Let's start with the title.

// TODO: rephrase

The simplest React component is a function which returns a HTML tag with a variable inside.

```js
// this JavaScript snippet is later referred to as <<heading-component>>
function Heading() {
  const title = 'Root finding web application';
  return <h1>{title}</h1>
}
```

The title component can be rendered using

```js
// this JavaScript snippet appenended to react/app.js
ReactDOM.render(
  <App/>,
  document.getElementById('container')
);
```

---
---
// TODO: show the result
```js
// this JavaScript snippet is later referred to as <<result-component>>
function Result(props) {
  const root = props.root;
  let message = 'Not submitted';
  if (root !== undefined) {
    message = 'Root = ' + root;
  }
  return <div id="answer">{message}</div>;
}
```

```js
// this JavaScript snippet appenended to react/app.js
function App() {
  // TODO: react state
  function handleSubmit(event) {
    // this JavaScript snippet is later referred to as <<handle-submit>>
    event.preventDefault();
    // this JavaScript snippet is appended to <<handle-submit>>
    const worker = new Worker('worker.js');
    // this JavaScript snippet is appended to <<handle-submit>>
    worker.postMessage({
      type: 'CALCULATE',
      payload: { epsilon: epsilon, guess: guess }
    });
    // this JavaScript snippet is appended to <<handle-submit>>
    worker.onmessage = function(message) {
        if (message.data.type === 'RESULT') {
          const result = message.data.payload.root;
          setRoot(result);
          worker.terminate();
      }
    };
  }

  return (
    // TODO: add form
  );
}
```

```js
  // FIXME: this is react state
  // this JavaScript snippet is later referred to as <<react-state>>
  const [epsilon, setEpsilon] = React.useState(0.001);
  // this JavaScript snippet is appended to <<react-state>>
  function onEpsilonChange(event) {
    setEpsilon(Number(event.target.value));
  }
  // this JavaScript snippet is appended to <<react-state>>
  const [guess, setGuess] = React.useState(-20);

  function onGuessChange(event) {
    setGuess(Number(event.target.value));
  }
  // this JavaScript snippet is appended to <<react-state>>
  const [root, setRoot] = React.useState(undefined);
```

```js
  // FIXME: this is react form
<div>
  <Heading/>
  { /* this JavaScript snippet is later referred to as <<react-form>> */ }
  <form onSubmit={handleSubmit}>
    <label>
      Epsilon:
      <input name="epsilon" type="number" value={epsilon} onChange={onEpsilonChange}/>
    </label>
    <label>
      Initial guess:
      <input name="guess" type="number" value={guess} onChange={onGuessChange}/>
    </label>
    <input type="submit" value="Submit" />
  </form>
  <Result root={root}/>
</div>
```