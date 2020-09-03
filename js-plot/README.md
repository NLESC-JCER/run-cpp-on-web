# Show me the visualization

Running C++ code in a web browser is all nice, but we really want to grab someones attention by visualizing something. In this blog we are going to make a plot from the results coming from our C++ code.

To make a plot we needs some data. In a [previous](../js-webapp) story we found the root of an equation using the NewtonRaphson algorithm implemented in C++ and compiled to a WebAsssembly module.
A single root value makes for a depressing plot. The NewtonRaphson algorithm uses iterations to find the root. So we will capture the data of each iteration and plot those.

Let make changes to the C++ code to retrieve the data from all of the iterations.

TODO add code snippets

TODO show iterations from intiaizas json doc

TODO Introduce Vega

TODO explain vega-spec.json layer construction

When we open the page we will be greeted by the following plot. We can zoom with scrollwheel and pan by dragging.

[![Image](vega-spec.svg)](https://nlesc-jcer.github.io/run-cpp-on-web/js-plot/index.html)
(Click on image to get interactive version)
TODO see if interactive version can be embedded
