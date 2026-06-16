// encapsulate surface info into a class

#include "Surface.hpp"
#include "Vec.inl"

Surface::Surface() {
	ambient_color = fVec3{0, 0, 0}; 
	ambient_scale_factor = 0;

	diffuse_color = fVec3{0, 0, 0}; 
	diffuse_scale_factor = 0;

	specular_color = fVec3{0, 0, 0}; 
	specular_scale_factor = 0;

	microfacet_shine = 0;

	reflection_color = fVec3{0, 0, 0};
	reflection_scale_factor = 0;
}

void Surface::precompute_variables(){
	scaled_ambient_color = ambient_color * ambient_scale_factor;
	ambient_scale_inverse = 1.0f - ambient_scale_factor;

	scaled_diffuse_color = diffuse_color * diffuse_scale_factor;

	scaled_specular_color = specular_color * ((microfacet_shine + 2.0f) / 8.0f); // Added cast bc errors
	specular_scale_inverse = 1.0f - specular_scale_factor;

	scaled_reflection_color = reflection_color * reflection_scale_factor;
	reflection_scale_inverse = 1.0f - reflection_scale_factor;
}
