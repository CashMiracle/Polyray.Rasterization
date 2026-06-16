#pragma once
// really basic type for vectors and colors
// just data definitions here, inline implementations in Vec.inl
// operators as template functions there, not members to support scalar*vector

#include <initializer_list>

// intentionally a POD (plain old data) type
// can have constructors & member functions, but nothing virtual
template <typename T, unsigned int N>
struct Vec {
	typedef T value_type;	// external typedef for template type

	T data[N];				// inline array of data

    // all elements initialized to one value
    Vec(T val = T(0));
    
	// constructor from list of appropriate type, used as Vec<T,N>{...}
    // or for the specialized types fVec3{x,y,z}
	Vec(std::initializer_list<T> rest);
    
    // array-like access must be public member functions
    // all other operations as template functions in Vec.inl
    T operator[](int i) const { return data[i]; }
    T &operator[](int i) { return data[i]; }
};

// specialized types for 3D vectors of float or byte
typedef Vec<float, 3>         fVec3;
typedef Vec<unsigned char, 3> bVec3;
