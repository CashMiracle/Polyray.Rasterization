#include "Config.hpp"

#define _USE_MATH_DEFINES

#include "World.hpp"
#include "Vec.inl"
#include "Config.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <vector>


using namespace std;




int main(int argc, char *argv[])
{
	if (argc <= 1) {
		cout << "Usage: " << argv[0] << " input.pi\n";
		return 1;
	}

	// platform-independent timing
	auto startTime = chrono::high_resolution_clock::now();

	// input and output files with paths
	string PIname = string(PROJECT_DATA_DIR) + argv[1];
	cout << "Input file: " << PIname << '\n';

	string PPMname = string(PROJECT_BUILD_DIR) + "raster.ppm";
	cout << "Output file: " << PPMname << '\n';

	// parse file into World object
	fstream pifile;
	pifile.open(PIname, ios::in);
	World world(pifile);

	// allocate data
	vector<bVec3> image(world.width * world.height, bVec(world.background));
	vector<fVec3> rawImage(world.width * world.height, world.background);
	vector<float> zBuffer(world.width * world.height, numeric_limits<float>::infinity());

	// compute viewing math according to raycast slide
	fVec3 dw = world.from - world.at;
	fVec3 u = normalize(cross(world.up, dw));
	fVec3 v = normalize(cross(dw, u));

	for(auto *obj : world.objects){
		fVec3 p0 = world.project(obj->P0, u, v, dw);
		fVec3 p1 = world.project(obj->P1, u, v, dw);
		fVec3 p2 = world.project(obj->P2, u, v, dw);

		Vec<float,2> v0 = {p0[0], p0[1]};
		Vec<float,2> v1 = {p1[0], p1[1]};
		Vec<float,2> v2 = {p2[0], p2[1]};

		float area = edgeFunction(v0, v1, v2);

		if(area <= 0){continue;} // backface

		int minX = max(0, (int)floor(min({p0[0], p1[0], p2[0]})));
		int maxX = min(world.width - 1, (int)ceil(max({p0[0], p1[0], p2[0]})));
		int minY = max(0, (int)floor(min({p0[1], p1[1], p2[1]})));
		int maxY = min(world.height - 1, (int)ceil(max({p0[1], p1[1], p2[1]})));


		for(int y = minY; y <= maxY; y++){
			for(int x = minX; x <= maxX; x++){
				Vec<float,2> p = {x + 0.5f, y + 0.5f}; // current pixel

				float w0 = edgeFunction(v1, v2, p);
				float w1 = edgeFunction(v2, v0, p);
				float w2 = edgeFunction(v0, v1, p);

				if (w0 >= 0 && w1 >= 0 && w2 >= 0){
					float a = w0 / area;
					float b = w1 / area;
					float c = w2 / area;

					float z = a * p0[2] + b * p1[2] + c * p2[2];

					int index = y * world.width + x;
					if(z < zBuffer[index] && z > world.hither){
						zBuffer[index] = z;
						fVec3 pPos = a * obj->P0 + b * obj->P1 + c * obj->P2;
						fVec3 pNorm = normalize(a * obj->N0 + b * obj->N1 + c * obj->N2);
						fVec3 V = normalize(world.from - pPos);

						
						fVec3 finalColor = obj->surface->ambient_color * obj->surface->ambient_scale_factor; 

						for(auto light : world.lights) {
							fVec3 L = normalize(light.location - pPos);
							fVec3 H = normalize(L + V);

							float nDotL = max(0.f, dot(pNorm, L));
							float vDotH = max(0.f, dot(V, H));
							float nDotH = max(0.f, dot(pNorm, H));

							float F = obj->surface->specular_scale_factor + (1.f - obj->surface->specular_scale_factor) * powf(1.f - nDotH, 5.f); 

							fVec3 specular = F * obj->surface->specular_color * powf(nDotH, obj->surface->microfacet_shine) * ((obj->surface->microfacet_shine + 2) / 8.f);
							fVec3 diffuse = (1.f - F) * obj->surface->diffuse_scale_factor * obj->surface->diffuse_color;
							finalColor = finalColor + light.color * nDotL * (diffuse + specular);
						}

						rawImage[index] = finalColor;
					}
				}
			}
		}

	}	


	// open ppm output file in build directory
	fstream ppmfile(PPMname, ios::out | ios::binary);
	if(ppmfile.fail()) {
		cerr << "Error opening " << PPMname << '\n';
		return 1;
	}

	for(int i = 0; i < world.width * world.height; i++){
		rawImage[i][0] = powf(clamp(rawImage[i][0], 0.f, 1.f), 1.f / 2.2f);
		rawImage[i][1] = powf(clamp(rawImage[i][1], 0.f, 1.f), 1.f / 2.2f);
		rawImage[i][2] = powf(clamp(rawImage[i][2], 0.f, 1.f), 1.f / 2.2f);

		image[i] = bVec(rawImage[i]);
	}

	ppmfile << "P6\n" << world.width << " " << world.height << "\n255\n";
	for (const auto& pixel : image) {
		ppmfile.write((char*)&pixel, 3);
	}

	// report final timing
	auto endTime = chrono::high_resolution_clock::now();
	chrono::duration<float> elapsed = endTime - startTime;
	cout << "Total time " << elapsed.count() << " seconds\n";
	return 0;
}
