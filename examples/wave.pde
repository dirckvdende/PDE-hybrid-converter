
pde {
    dims [x];
    domain x * x < 100;
    pivot [0];
    scale 0.08;

    equation dt(dt(u)) = dx(dx(u));
    init u = sin(x * 3.1415 / 10);
    init dt(u) = 0;
    boundary u = 0;
    interval u = [-2, 2];
    interval dt(u) = [-2, 2];

    time 20;
    iterations 1;
    emit u as u;
}