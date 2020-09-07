function Heading() {
    const title = 'Root finding web application';
    return <h1>{title}</h1>
  }

  function Result(props) {
    const root = props.root;
    let message = 'Not submitted';
    if (root !== undefined) {
      message = 'Root = ' + root;
    }
    return <div id="answer">{message}</div>;
  }

  function App() {
    const [tolerance, setTolerance] = React.useState(0.001);
    const [initial_guess, setGuess] = React.useState(-4);

    function onToleranceChange(event) {
      setTolerance(Number(event.target.value));
    }

    function onGuessChange(event) {
      setGuess(Number(event.target.value));
    }
    const [root, setRoot] = React.useState(undefined);

    function handleSubmit(event) {
      event.preventDefault();
      const worker = new Worker('worker.js');
      worker.postMessage({
        type: 'CALCULATE',
        payload: { epsilon: tolerance, guess: initial_guess }
      });
      worker.onmessage = function(message) {
          if (message.data.type === 'RESULT') {
            const result = message.data.payload.root;
            setRoot(result);
            worker.terminate();
        }
      };
    }

    return (
      <div>
        <Heading/>
        <form onSubmit={handleSubmit}>
          <label>
            Tolerance:
            <input name="tolerance" type="number" value={tolerance} onChange={onToleranceChange}/>
          </label>
          <label>
            Initial guess:
            <input name="initial_guess" type="number" value={initial_guess} onChange={onGuessChange}/>
          </label>
          <input type="submit" value="Submit" />
        </form>
        <Result root={root}/>
      </div>
    );
  }
  ReactDOM.render(
    <App/>,
    document.getElementById('container')
  );