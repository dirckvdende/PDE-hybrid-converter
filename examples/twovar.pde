
pde {
    dims [x];
    domain x * x < 1;
    pivot [0, 0];
    scale 0.1;

    equation dt(u) = v;
    equation dt(v) = u;
    init u = 1;
    init v = 1;
    boundary u = 1;
    boundary v = 1;
    interval u = [-100, 100];
    interval v = [-100, 100];

    time 10;
    iterations 1;
}