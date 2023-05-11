
pde heat {
    dimensions x, y;
    equation dt(u) = dx(dx(u)) + dy(dy(u));
    func u;
    domain x * x + y * y < 1;
    pivot 0, 0;
    scale 0.1;
    boundary 1;
    init x * x + y * y;
    time 10;
    iterations 20;
}