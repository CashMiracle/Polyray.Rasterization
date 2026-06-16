// implementation of ray/sphere intersections

#include "Sphere.hpp"
#include "Vec.inl"
#include <limits>

int Sphere::count = 0;

Sphere::Sphere(fVec3 _center, float _radius, const Surface *_surface)
: Object(_surface)
{
    center = _center;
    radius = _radius;
    
    ++count;
}

float Sphere::hit(Ray ray) const
{
    // quadratic equation for intersection
    fVec3 g = ray.start - center;
    float b = dot(ray.direction, g);
    float c = dot(g, g) - radius*radius;
    
    // did we even hit the sphere?
    float disc = b*b - c;
    if (disc < 0.f) return Ray::infinity;
    disc = sqrtf(disc);
    
    float t0 = (-b - disc);
    if (t0 > ray.tMin && t0 < ray.tMax) return t0;
    
    float t1 = (-b + disc);
    if (t1 > ray.tMin && t1 < ray.tMax) return t1;
    
    return Ray::infinity;
}

fVec3 Sphere::get_norm(fVec3 hit_location) const
{
	return normalize(hit_location - center);
}
