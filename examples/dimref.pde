
pde {
    dims [x];
    domain x * x < 50;
    pivot [0];
    scale 0.08;

    equation dt(u) = x;
    init u = 0;
    boundary u = 0;
    interval u = [0, 1e10];

    time 20;
    iterations 1;
    emit u as u;
}