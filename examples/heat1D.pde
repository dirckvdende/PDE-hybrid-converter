
pde {
    dims [x];
    domain x * x < 1;
    pivot [0, 0];
    scale 0.4;

    equation dt(u) = dx(dx(u));
    init u = 1 - x * x / 2 + x * x * x * x / 24;
    boundary u = 1;
    interval u = [-10, 10];

    time 20;
    iterations 5;
    emit x as x;
}