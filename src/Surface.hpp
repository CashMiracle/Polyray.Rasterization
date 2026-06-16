// encapsulate surface info into a class
#pragma once

#include "Vec.hpp"

struct Surface {
	fVec3 ambient_color;
	float ambient_scale_factor;
	float ambient_scale_inverse;
	fVec3 scaled_ambient_color;

	fVec3 diffuse_color; 
	float diffuse_scale_factor;
	fVec3 scaled_diffuse_color;

	fVec3 specular_color; 
	float specular_scale_factor;
	float specular_scale_inverse;
	fVec3 scaled_specular_color;

	float microfacet_shine;

	fVec3 reflection_color;
	float reflection_scale_factor;
	float reflection_scale_inverse;
	fVec3 scaled_reflection_color;

	Surface();
	void precompute_variables();
};
