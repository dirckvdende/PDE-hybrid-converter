
pde {
    dims [x];
    domain x * x < 100;
    pivot [0];
    scale 0.08;

    equation dt(u) = v;
    equation dt(v) = dx(dx(u));
    init u = sin(3.1415 / 10 * x);
    init v = 0;
    boundary u = 0;
    boundary v = 0;
    interval u = [-2, 2];
    interval v = [-2, 2];

    time 20;
    iterations 1;
    emit u as u;
}