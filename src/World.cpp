// implementation code for World object
// World holds everything we know about the world.
// Code here initializes it based on file input

#include "World.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Ray.hpp"
#include "Vec.inl"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// scoped global for what is enabled
World::Features World::enables;

// feature defaults
World::Features::Features()
{
	spheres = triangles = polygons = true;
}

// local function to parse a token starting with a float (ignore any trailing , or >)
static float readFloat(istream &istr)
{
	string token;
	istr >> token;
	return stof(token);
}

// local function to parse a token starting with an int (ignoring any trailing ,)
static int readInt(istream &istr)
{
	string token;
	istr >> token;
	return stoi(token);
}

// local function to parse set of 3 tokens <x, y, z> into a float vector
static fVec3 readVector(istream &istr)
{
	string token;
	fVec3 r = {1, 1, 1};

	// process x, skipping leading <
	istr >> token;
	if(token == "white,"){
		return r;
	}
	r[0] = stof(token.substr(1));

	// process y and z
	r[1] = readFloat(istr);
	r[2] = readFloat(istr);

	return r;
}

World::World(istream &istr)
{
	auto startTime = chrono::high_resolution_clock::now();

	// defaults
	background = fVec3{0,0,0};
	from       = fVec3{0,0,-1};
	at         = fVec3{0,0,0};
	up         = fVec3{0,1,0};
	fov = 45.f;
	aspect = -1.f;
	hither = 0.001f;
	width = height = 256;

	string token;
	while (istr >> token) {
		// background and viewpoint info
		if (token == "background")
			background = readVector(istr);
		else if (token == "from")
			from = readVector(istr);
		else if (token == "at")
			at = readVector(istr);
		else if (token == "up")
			up = readVector(istr);
		else if (token == "angle")
			fov = readFloat(istr);
		else if (token == "aspect")
			aspect = readFloat(istr);
		else if (token == "hither")
			hither = readFloat(istr);
		else if (token == "resolution") {
			width = readInt(istr);
			height = readInt(istr);
		}

		// surface definition
		else if (token == "define") {
			string surfName;
			istr >> surfName;
			Surface surfValue;

			// scan for diffuse keyword or close of define block
			while (istr >> token) {
				if (token == "diffuse") {
					surfValue.diffuse_color = readVector(istr);
					surfValue.diffuse_scale_factor = readFloat(istr);
				} else if (token == "ambient"){
					surfValue.ambient_color = readVector(istr);
					surfValue.ambient_scale_factor = readFloat(istr);
				} else if (token == "specular"){
					surfValue.specular_color = readVector(istr);
					surfValue.specular_scale_factor = readFloat(istr);
				} else if (token == "microfacet") {
					istr >> token; // Phong
					surfValue.microfacet_shine = readFloat(istr);
				} else if (token == "reflection") {
					surfValue.reflection_color = readVector(istr);
					surfValue.reflection_scale_factor = readFloat(istr);
				} else if (token == "}")
					break;
			}

			// add to surface map
			surfValue.precompute_variables();
			surfaces[surfName] = surfValue;
		}

		// object definition
		else if (token == "object") {
			istr >> token;		// open brace
			istr >> token;		// object type
			if (token == "sphere" && World::enables.spheres) {
				// sphere geometry
				fVec3 center = readVector(istr);
				float radius = readFloat(istr);

				// surface to use
				istr >> token;
				Surface *surf = &surfaces[token];

				// add sphere to list
				triangle_sphere(radius, center, surf);
			}
			else if (token == "polygon" && (World::enables.triangles || World::enables.polygons)) {
				// pre-read all vertices
				int numVerts = readInt(istr);
				vector<fVec3> verts;
				verts.reserve(numVerts);
				for (int i=0; i < numVerts; ++i) {
					verts.push_back(readVector(istr));
				}

				// surface to use
				istr >> token;
				Surface *surf = &surfaces[token];

				// compute normal from first 3 vertices
				if (numVerts < 3) continue;
				fVec3 N = normalize(cross(verts[1]-verts[0], verts[2]-verts[0]));

				for(int i = 0; i < numVerts - 2; i++){
					objects.push_back(new Triangle(verts[i], verts[i+1], verts[i+2], N, N, N, surf));
				}
			}
		}
		else if (token == "light") {
			Light temp;
			temp.color = readVector(istr);
			temp.location = readVector(istr);
			lights.push_back(temp);
		}
	}

	// handle auto-aspect ratio
	if (aspect < 0.f)
		aspect = width/height;

	// report some stats
	auto endTime = chrono::high_resolution_clock::now();
	chrono::duration<float> elapsed = endTime - startTime;
	cout << "File read " << elapsed.count() << " seconds\n";
	cout << "\n";
}


// Shadow helper function
bool World::in_shadow(Light test_light, fVec3 hit_point){
	// Slightly offset ray to not hit the object
	Ray shadow_ray = Ray(hit_point + hither * normalize(test_light.location - hit_point), normalize(test_light.location - hit_point));

	// Get ray object hit
	Object *closest = nullptr;
	for (Object *obj : objects) {
		float t = obj->hit(shadow_ray);
		if (t < shadow_ray.tMax && t > hither) {
			float light_distance = length(test_light.location - hit_point);
			if (t < light_distance){
				return true;
			}
		}
	}

	return false;
}


fVec3 World::project(fVec3 vert, fVec3 u, fVec3 v, fVec3 dw){
	fVec3 rel = vert - from;

	fVec3 cam = {dot(rel, u), dot(rel, v), dot(rel, -normalize(dw))};	

	float dist = length(dw);
	Vec<float, 2> proj = {(cam[0] * dist) / cam[2], (cam[1] * dist) / cam[2]};

	float top = dist * tanf(fov * M_PI / 360.f);
	float right = top * aspect;

	Vec<float, 2> screen = {(proj[0] + right) * (width / (2.f * right)), (top - proj[1]) * (height / (2.f * top))};

	return fVec3{screen[0], screen[1], cam[2]};
}


void World::triangle_sphere(float size, fVec3 trans, Surface* texturePPM){
	vector<fVec3> verts, norm;
	vector<int> indices;
	int h = 32;
	int w = 32;
	// build vertex, normal and texture coordinate arrays
	// * x & y are longitude and latitude grid positions
	for(unsigned int y=0;  y <= h;  ++y) {
		for(unsigned int x=0;  x <= w;  ++x) {
			// Texture coordinates scaled from x and y. Be sure to cast before division!
			float u = float(x)/float(w), v = float(y) / float(h);

			// normal for sphere is normalized position in spherical coordinates
			float cx = cosf(2.f * M_PI * u), sx = sinf(2.f * M_PI * u);
			float cy = cosf(M_PI * v), sy = sinf(M_PI * v);
			fVec3 N = fVec3{cx * sy, sx * sy, cy};
			norm.push_back(N);

			// 3d vertex location scaled by sphere size
			verts.push_back(size * N);
		}
	}

	// build index array linking sets of three vertices into triangles
	// two triangles per square in the grid. Each vertex index is
	// essentially its unfolded grid array position. Be careful that
	// each triangle ends up in counter-clockwise order
	for(unsigned int y=0; y<h; ++y) {
		for(unsigned int x=0; x<w; ++x) {


			indices.push_back((w+1)* y    + x);
			indices.push_back((w+1)*(y+1) + x+1);
			indices.push_back((w+1)* y    + x+1);

			indices.push_back((w+1)* y    + x);
			indices.push_back((w+1)*(y+1) + x);
			indices.push_back((w+1)*(y+1) + x+1);
		}
	}


	// Translate orb
	for(int i = 0; i < (h+1)*(w+1); i++){
		verts[i] = verts[i] + trans;
	}
	
	
	for(int i = 0; i < indices.size(); i += 3){
		int i0 = indices[i]; 
		int i1 = indices[i+1]; 
		int i2 = indices[i+2];
	objects.push_back(new Triangle(verts[i0], verts[i1], verts[i2], norm[i0], norm[i1], norm[i2], texturePPM));
	}
}


World::~World(){
	for(auto obj: objects){
		delete obj;
	}
}
