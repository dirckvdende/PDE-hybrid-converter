
pde {
    dims [x];
    domain x * x < 50;
    pivot [0];
    scale 0.08;

    equation dt(u) = dx(dx(u));
    init u = 50 - x * x;
    boundary u = 0;
    interval u = [-50, 50];

    time 20;
    emit u as u;
}