
pde {
    # Dimension names
    dims [x];
    # Domain is the interval (-sqrt(50), sqrt(50))
    domain x * x < 50;
    # Point 0 is in this interval
    pivot [0];
    # One grid point is 0.08 units large
    scale 0.08;

    # Heat equation
    equation dt(u) = dx(dx(u));
    # Initial value at t = 0
    init u = 50 - x * x;
    # Boundary condition at x = +-sqrt(50)
    boundary u = 0;
    # Interval of expected output values
    interval u = [-50, 50];

    # Run until t = 20
    time 20;
    # Number of iterations, in case of iterative model
    iterations 1;
    # Output values of u
    emit u as u;
}