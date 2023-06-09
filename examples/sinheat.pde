
pde {
    dims [x];
    domain x * x < 100;
    pivot [0];
    scale 0.08;

    equation dt(u) = 2 * dx(dx(u));
    init u = sin(x);
    boundary u = 0;
    interval u = [-1e100, 1e100];

    time 1;
    iterations 5;
    emit u as u;
}