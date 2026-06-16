#pragma once
// inline implementation code for Vec class

// based on ideas from Nathan Reed
//        http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/
//
// This file contains function declarations and inline function definitions.
// The code is mostly boring and repetitive. If you need a function and it
// isn't here, go ahead and add it!
//
// Note that since the size is a template parameter, it's a compile-time
// constant. Loops with N iterations should be unrolled by the compiler
//
// Math operations that apply to each element (s = scalar; v, v1, v2 = vectors)
//       v1+v2, v1-v2, v1*v2, v1/v2
//       -v, s+v, v+s, s-v, v-s, s*v, v*s, s/v, v/s
//
// Linear algebra and other handy functions
//       dot(v1,v2): N-dimensional vector dot product
//       length(v): Euclidean vector length
//       normalize(v): unit-length vector in the direction of v
//		 clamp(v,min,max): clamp all values to min..max
//       pow(v,e): raise each component to the e power
//       cross(v1,v2): vector cross product -- v1 and v2 must both be Vec<3>
//       dimensions(v): size of N (e.g. 2 for Vec<2>, 3 for Vec<3>, etc.)
//
// Explicit conversion from float vector to byte vector
//       bv bVec(fv)

#include "Vec.hpp"
#include <cassert>
#include <cmath>

// all elements initialized to one value
template <typename T, unsigned int N>
Vec<T,N>::Vec(T val) {
    for (int i=0; i<N; ++i) {
        data[i] = val;
    }
}

// constructor from list of appropriate type
template <typename T, unsigned int N>
Vec<T,N>::Vec(std::initializer_list<T> values) {
    assert(values.size() == N);
    int i = 0;
    for (auto v = values.begin(); i<N; ++i, ++v) {
        data[i] = *v;
    }
}

//////////////////////////////////////////////////////////////////////
// component-wise operations: +, -, *, /, %
template <typename T, unsigned int N>
inline Vec<T,N> operator+(Vec<T,N> v1, Vec<T,N> v2) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] + v2[i];
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator-(Vec<T,N> v1, Vec<T,N> v2) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] - v2[i];
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator*(Vec<T,N> v1, Vec<T,N> v2) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] * v2[i];
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator/(Vec<T,N> v1, Vec<T,N> v2) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] / v2[i];
	return v;
}

//////////////////////////////////////////////////
// operations between a scalar and vector: +, -, *, /
template <typename T, unsigned int N>
inline Vec<T,N> operator-(Vec<T,N> v1) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = -v1[i];
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator+(T s, Vec<T,N> v1) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = s + v1[i];
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator-(T s, Vec<T,N> v1) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = s - v1[i];
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator*(T s, Vec<T,N> v1) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = s * v1[i];
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator/(T s, Vec<T,N> v1) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = s / v1[i];
	return v;
}

//////////////////////////////////////////////////
// operations between a vector and a scalar: +, -, *, /
template <typename T, unsigned int N>
inline Vec<T,N> operator+(Vec<T,N> v1, T s) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] + s;
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator-(Vec<T,N> v1, T s) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] - s;
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator*(Vec<T,N> v1, T s) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] * s;
	return v;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator/(Vec<T,N> v1, T s) {
	Vec<T,N> v;
	for(int i=0; i<N; ++i)
		v[i] = v1[i] / s;
	return v;
}

//////////////////////////////////////////////////////////////////////
// other linear algebra operations

// vector dot product, dot(v1, v2)
template <typename T, unsigned int N>
inline float dot(Vec<T,N> v1, Vec<T,N> v2) {
	float v = 0;
	for(int i=0; i<N; ++i)
		v += v1[i] * v2[i];
	return v;
}

// vector length, length(v1)
template <typename T, unsigned int N>
inline float length(Vec<T,N> v1) {
	return std::sqrt(dot(v1, v1));
}

// normalized vector, normalize(v1)
template <typename T, unsigned int N>
inline Vec<T,N> normalize(Vec<T,N> v1) {
	return v1 / length(v1);
}

// clamp to min..max, clamp(v1,min,max)
template <typename T, unsigned int N>
inline Vec<T,N> clamp(Vec<T,N> v1, T min, T max) {
	Vec<T,N> v;
	for (int i=0; i<N; ++i)
		v[i] = v1[i] < min ? min : v1[i] > max ? max : v1[i];	
	return v;
}

// raise all elements to a power, pow(v1, e)
// only for types where pow is defined (float, double, ...)
template <typename T, unsigned int N>
inline Vec<T,N> pow(Vec<T,N> v1, T e) {
	Vec<T,N> v;
	for (int i=0; i<N; ++i)
		v[i] = std::pow(v1[i],e);	
	return v;
}

// cross product, cross(v1,v2) for Vec<3> only
template <typename T>
inline Vec<T,3> cross(Vec<T,3> v1, Vec<T,3> v2) {
	Vec<T,3> v;
	v[0] = v1[1]*v2[2] - v1[2]*v2[1];
	v[1] = v1[2]*v2[0] - v1[0]*v2[2];
	v[2] = v1[0]*v2[1] - v1[1]*v2[0];
	return v;
}

template <typename T, unsigned int N>
inline unsigned int dimensions(Vec<T,N> v) {
	return N;
}

//////////////////////////////////////////////////////////////////////
// Explicit conversion from float to byte
template<unsigned int N>
inline Vec<unsigned char,N> bVec(Vec<float,N> v1) {
    Vec<unsigned char,N> v;
    for (int i=0; i<N; ++i)
        v[i] = (unsigned char)(255.f * v1[i]);
    return v;
}



//////////////////////////////////////////////////////////////////////
/// Barycentric Coordinate Functions

inline float edgeFunction(Vec<float,2> a, Vec<float,2> b, Vec<float,2> c){
	return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}



