
# Input format

A filename should be passed to the compiler. This file contains a PDE specification in the following format:
```
pde name {
    optionA valueA;
    optionB valueB;
    ...
}
```
The following options have to be filled in:
| Name | Description |
| --- | --- |
| `dimensions` | Names of the spatial dimensions, the temporal dimension is always `t`. |
| `equation` | The PDE equation to simulate. |
| `func` | The name of the function to solve for in the given PDE. |
| `domain` | An equation in the dimension names, which should evaluate to `true` on the domain and `false` outside it. The domain cannot be too large and should be connected. |
| `pivot` | A point inside the domain that is used as a starting point for the compiler. |
| `scale` | The scale of one cell in the approximation grid. |
| `boundary` | Value outside the domain, that is used as a boundary condition for the PDE. |
| `init` | Initial value of the function at `t = 0`. |
| `time` | The time interval to simulate. |
| `iterations` | The number of iterations to run the simulation for. |
