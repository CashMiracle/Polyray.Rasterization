#pragma once
// sphere object

#include "Object.hpp"
#include "Vec.hpp"

class Sphere : public Object {
	fVec3 center;
	float radius;

public:
    static int count;   // total count of spheres

    Sphere(fVec3 _center, float _radius, const Surface *_surface);

    // compute t for hit, or infinity no hit
    virtual float hit(Ray ray) const override;
    virtual fVec3 get_norm(fVec3 hit_location) const override;
};
