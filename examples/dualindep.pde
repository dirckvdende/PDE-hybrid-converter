
pde {
    dims [x];
    domain x * x < 50;
    pivot [0];
    scale 0.08;

    equation dt(u) = 0;
    equation dt(v) = 1;
    init u = 0;
    init v = 0;
    boundary u = 0;
    boundary v = 0;
    interval u = [-50, 50];
    interval v = [-50, 50];

    time 20;
    iterations 1;
    emit u as val;
}