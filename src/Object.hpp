#pragma once
// base class for all objects

#include "Ray.hpp"

struct Surface;

class Object {
public:
	const Surface *surface;
    
    Object(const Surface *_surface) : surface(_surface) {}
    
    // compute t for hit, or infinity no hit
    virtual float hit(Ray ray) const = 0;
	
    // Get normal of object collision
    virtual fVec3 get_norm(fVec3 hit_location) const = 0;
};
