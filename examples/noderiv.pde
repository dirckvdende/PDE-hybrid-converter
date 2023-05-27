
pde {
    dims [x, y];
    domain x * x + y * y < 1;
    pivot [0, 0];
    scale 0.05;

    equation dt(u) = x + y;
    init u = x * x + y * y;
    boundary u = 1;
    interval u = [0, 1];

    time 10;
    iterations 3;
}