
pde {
    dims [x];
    domain x > -0.7782 and x < 1.4367;
    pivot [0];
    scale 0.01;

    equation dt(dt(u)) = dx(dx(u));
    init u = exp(-(x * x));
    init dt(u) = 0;
    boundary u = 0;
    interval u = [-1e20, 1e20];
    interval dt(u) = [-1e20, 1e20];

    time 1.5;
    iterations 1;
    emit u as u;
}