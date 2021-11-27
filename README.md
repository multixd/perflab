# perflab

Performance lab, of basic 3D stencil calculation.
Take advantage of spatial and temporal locality by reording the order of loops.
Attempted loop tiling for longer instances of S, so to ensure our cache doesn't miss.

Making a specific function for each test greatly increases the performance of the algorithm, 
as the compiler figures out how to vectorize data when the S value is specified.
