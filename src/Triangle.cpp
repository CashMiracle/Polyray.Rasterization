// implementation of ray/triangle intersections

#include "Triangle.hpp"
#include "Vec.inl"

int Triangle::count = 0;

// constructor, incuding derived data
Triangle::Triangle(fVec3 _P0, fVec3 _P1, fVec3 _P2, fVec3 _N0, fVec3 _N1, fVec3 _N2, const Surface *_surface)
: Object(_surface)
{
    P0 = _P0; P1 = _P1; P2 = _P2;
    N0 = _N0; N1 = _N1; N2 = _N2;

    // precompute for barycentrics, see slides
    Na = cross(N0, P2-P1);
    Nb = cross(N0, P0-P2);
    ha = dot(Na, P0-P1);
    hb = dot(Nb, P1-P2);
    NdotP0 = dot(N0, P0);
    
    ++count;
}

float Triangle::hit(Ray ray) const
{
    // where does ray hit plane of triangle?
    float t = (NdotP0 - dot(N0, ray.start)) / dot(N0, ray.direction);
    
    // is t in range?
    if (t <= ray.tMin || t >= ray.tMax) return Ray::infinity;
    fVec3 P = ray.start + t * ray.direction;

    // compute and check alpha
    float a = dot(Na, P-P1) / ha;
    if (a < 0 || a > 1) return Ray::infinity;
    
    // compute and check beta
    float b = dot(Nb, P-P2) / hb;
    if (b < 0 || b > 1) return Ray::infinity;
    
    // compute and check gamma
    float c = 1 - a - b;
    if (c < 0) return Ray::infinity;
    
    // return hit
    return t;
}

fVec3 Triangle::get_norm(fVec3 hit_location) const
{
	return N0;
}
