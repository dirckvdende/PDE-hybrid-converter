
pde {
    dims [x];
    domain x * x < 20;
    pivot [0];
    scale 0.08;

    equation dt(dt(u)) = dx(dx(u));
    init u = sin(x);
    init dt(u) = 0;
    boundary u = 0;
    interval u = [-50, 50];
    interval dt(u) = [-50, 50];

    time 20;
    iterations 1;
    emit u as u;
}