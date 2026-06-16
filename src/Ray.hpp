#pragma once
// ray start point, direction, and min and max extents

#include "Vec.hpp"
#include <limits>

class Ray {
public:
	fVec3 start;
	fVec3 direction; // assumed to be normalized!
	float tMin, tMax;
    static constexpr float infinity = std::numeric_limits<float>::infinity();

	Ray(fVec3 _start, fVec3 _direction, float _tMin = 0.001f, float _tMax = infinity) 
	: start(_start), direction(_direction), tMin(_tMin), tMax(_tMax) {}
};
