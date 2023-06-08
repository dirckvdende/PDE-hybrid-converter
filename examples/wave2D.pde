
pde {
    dims [x];
    domain x * x < 20;
    pivot [0];
    scale 0.08;

    equation dt(u) = v;
    equation dt(v) = dx(dx(u));
    init u = sin(x);
    init v = 0;
    boundary u = 0;
    boundary v = 0;
    interval u = [-1e40, 1e40];
    interval v = [-1e40, 1e40];

    time 1;
    iterations 1;
    emit u as u;
}