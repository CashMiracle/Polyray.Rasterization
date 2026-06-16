# Polyray.Rasterization
Barycentric triangle rasterization program written in C++ using no external libraries, only STD templates. Converts polyray text files into ppm files. Accounts for back face culling and uses z-buffering.

The only directly-rendered primitive is a triangle with interpolated per-vertex normals. Computes the baycentric equation coefficients and screen-space rectangle bounding the transformed vertex locations; then loop over the bounding rectangle pixels, using incremental updates of the barycentric α, β, and γ. Uses α, β, and γ as decision variables to tell which pixels are inside the triangle, weights for weighted interpolation of the normals, pixel depth for z-buffering, and world-space position.

Several of the sample files include spheres. They are tesselated into a collection of triangles. 

Any convex polygons are converted into a set of triangles. Concave polygons are not handled. The sample files do not have per-vertex normals for triangles or polygons so the same normal-interpolating triangle primitive is used for everything.

Handles polyray keywords for ambient, diffuse, specular, microfacet, and reflection, resulting in the computation of the surface color for a ray. It finds the first intersection along the ray, and returns a weighted sum of the direct illumination from each light and the color.
