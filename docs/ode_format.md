
# ODE specification file format

An ODE specification file can contain one or more systems of ODEs. Variables can be declared using `var`, with after it an `integ` expression or just a normal expression.

## Example

Suppose we want to encode the following ODE:
$$
    \dot x = y, \qquad x(0) = 1, \\
    \dot y = -x, \qquad y(0) = 0.
$$
We use the variables `x` and `y` and define these using `integ` expressions. To store the values that `x` and `y` admit during the simulation, we use the `emit` keyword. Also, the interval at which `x` and `y` are defined has to be specified. If the solution of the ODE moves out of these intervals, the behaviour is undefined. For this ODE we already know the solutions will never exceed the interval $[-1, 1]$. Lastly we set the time interval of the simulation to $[0, 20]$. The result is the following:
```
system {
    var x = integ(y, 1);
    var y = integ(-x, 0);
    emit x as xpos;
    emit y as ypos;
    interval x = [-1, 1];
    interval y = [-1, 1];
    time 20;
}
```

## Auxiliary variables

A variable does not have to be defined with an `integ` expression. If it is not, it cannot depend on variables that have not yet been defined however. Hence the following is valid:
```
system {
    var x = integ(y, 1);
    var y = x;
    ...
}
```
But this is not:
```
system {
    var y = x;
    var x = integ(y, 1);
    ...
}
```

## Multiple systems of ODEs

The `emit` setting can be used to store results from one simulation and use them in the next. For example:
```
system {
    var x = integ(x, 1);
    emit x as x;
    interval x = [0, 10];
    time 10;
}

system {
    var y = x + 1;
    emit y as y;
    interval y = [1, 11];
    time 10;
}
```
If the `emit` setting is not used, then the values that a variable admits during the simulation will be lost of the single system is simulated.