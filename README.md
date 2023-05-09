
# PDE-hybrid-converter

Compiler that converts an input PDE with sufficient boundary conditions to an iterated ODE model. This model should then serve as an input to an analog-digital hybrid computer. The converter executes the following stages:

1. Create a grid that fits the input domain and determine which cells are boundary/internal cells
2. Divide the grid input groups that are processed in parallel
3. Divide simulation time interval into intervals of which an upper bound is known
4. Generate ODE on the grid, which serves as an estimate for the input PDE
5. Output configurations to file