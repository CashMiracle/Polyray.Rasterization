#pragma once
// NxN-dimensional matrix class template
// based on ideas from Nathan Reed
//    http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/

#include "Vec.hpp"
#include <initializer_list>

/////////////////////////////////////////////////////////////////////
// NxN matrix
template <typename T, unsigned int N>
struct Mat {
    typedef T value_type;

    Vec<T,N> data[N];

    // matrix with val on diagonal
    Mat(T val = T(0));

	// constructor from list of elements, as Mat<T,N>{...}
    // or for the specialized types fMat4{a00,a01,...,a33}
	Mat(std::initializer_list<T> rest);
    
    // constructor from list of vectors
    Mat(std::initializer_list<Vec<T,N> > rest);
    
    // array-like access must be public member functions
    // all other operations as template functions in Vec.inl
    Vec<T,N> operator[](int i) const { return data[i]; }
    Vec<T,N> &operator[](int i) { return data[i]; }
};
