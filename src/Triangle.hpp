#pragma once
// triangle object

#include "Object.hpp"

class Triangle : public Object {
public:
	fVec3 P0, P1, P2;    // vertices
    fVec3 N0, N1, N2;             // normal
    fVec3 Na, Nb;        // edge normals
    float ha, hb;           // triangle heights
    float NdotP0;

public:
    static int count;   // total count of triangles

    Triangle(fVec3 _P0, fVec3 _P1, fVec3 _P2, fVec3 _N0, fVec3 _N1, fVec3 _N2, const Surface *_surface);
    
    // compute t for hit, or infinity no hit
    virtual float hit(Ray ray) const override;
    virtual fVec3 get_norm(fVec3 hit_location) const override;
};
