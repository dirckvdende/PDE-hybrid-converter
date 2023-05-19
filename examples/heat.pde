
pde heat {
    dims [x, y];
    domain x * x + y * y < 1;
    pivot [0, 0];
    scale 0.1;

    vars u;
    equation dt(u) = dx(dx(u)) + dy(dy(u));
    init u = x * x + y * y;
    boundary u = 1;
    interval u = [0, 1];

    time 10;
    iterations 20;
}