// Everything we know about the world
#pragma once

#include "Vec.hpp"
#include "Surface.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Ray.hpp"
#include <istream>
#include <map>
#include <list>
#include <string>

class Object;

class World {
public:
    // feature flags
    struct Features {
        bool spheres : 1;
        bool triangles : 1;
        bool polygons : 1;

        Features();
    };
    static Features enables;

    fVec3 background;	    // background color
	fVec3 from;			    // eye location
	fVec3 at;			    // look-at location
	fVec3 up;			    // up vector
    float fov;				// field of view
	float aspect;			// aspect ratio
	float hither;			// closest intersection
	int width, height;		// image dimensions

	// map from string name to surface description
	// for now, just the diffuse color
	std::map<std::string, Surface> surfaces;

	// list of objects in the scene
	std::list<Triangle*> objects;
	
	// list of lights in the scene
	std::list<Light> lights;
public:
    // constructor: read world data from a file
    World(std::istream &ifile); 
    bool in_shadow(Light test_light, fVec3 target);
    fVec3 project(fVec3 vert, fVec3 u, fVec3 v, fVec3 dw);
    void triangle_sphere(float size, fVec3 trans, Surface* texturePPM);
    ~World();
};
