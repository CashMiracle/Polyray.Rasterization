#pragma once
// NxN-dimensional matrix class template
// based on ideas from Nathan Reed
//    http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/

#include "Mat.hpp"
#include "Vec.inl"
#include <cmath>
#include <cassert>

//////////////////////////////////////////////////////////////////////
// constructors
// all elements initialized to one value
template <typename T, unsigned int N>
Mat<T,N>::Mat(T val) {
    for (int i=0; i<N; ++i) {
	    for (int j=0; j<N; ++j) {
      		data[i][j] = T(0);
    	}
        data[i][i] = val;
    }
}

// constructor from list of elements
template <typename T, unsigned int N>
Mat<T,N>::Mat(std::initializer_list<T> values) {
    assert(values.size() == N*N);
    int i = 0;
    T* dataptr = (T*)data;
    for (auto v = values.begin(); i<N*N; ++i, ++v) {
        dataptr[i] = *v;
    }
}

// constructor from list columns
template <typename T, unsigned int N>
Mat<T,N>::Mat(std::initializer_list<Vec<T,N> > cols) {
    assert(cols.size() == N);
    int i = 0;
    for (auto v = cols.begin(); i<N; ++i, ++v) {
        data[i] = *v;
    }
}

//////////////////////////////////////////////////////////////////////
// matrix size
template <typename T, unsigned int N>
inline int dimensions(const Mat<T,N> &m) {
    return N;
}

//////////////////////////////////////////////////////////////////////
// matrix addition and subtraction
template <typename T, unsigned int N>
inline Mat<T,N> operator+(Mat<T,N> m1, Mat<T,N> m2) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            result[i][j] = m1[i][j] + m2[i][j];
    return result;
}

template <typename T, unsigned int N>
inline Mat<T,N> operator-(Mat<T,N> m1, Mat<T,N> m2) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            result[i][j] = m1[i][j] - m2[i][j];
    return result;
}

//////////////////////////////////////////////////////////////////////
// scalar multiplication and division
template <typename T, unsigned int N>
inline Mat<T,N> operator-(Mat<T,N> m) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            result[i][j] = -m[i][j];
    return result;
}

template <typename T, unsigned int N>
inline Mat<T,N> operator*(T s, Mat<T,N> m) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            result[i][j] = s * m[i][j];
    return result;
}

template <typename T, unsigned int N>
inline Mat<T,N> operator*(Mat<T,N> m, T s) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            result[i][j] = m[i][j] * s;
    return result;
}

template <typename T, unsigned int N>
inline Mat<T,N> operator/(Mat<T,N> m, T s) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            result[i][j] = m[i][j] / s;
    return result;
}

//////////////////////////////////////////////////////////////////////
// matrix*matrix, matrix*vector, and vector*matrix
template <typename T, unsigned int N>
inline Mat<T,N> operator*(Mat<T,N> m1, Mat<T,N> m2) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i) {
        for(int j=0; j<N; ++j) {
            result[j][i] = m1[0][i] * m2[j][0];

            for(int k=1; k<N; ++k)
                result[j][i] += m1[k][i] * m2[j][k];
        }
    }
    return result;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator*(Vec<T,N> v, Mat<T,N> m) {
    Vec<T,N> result;
    for(int i=0; i<N; ++i) {
        result[i] = v[0] * m[i][0];

        for(int j=1; j<N; ++j)
            result[i] += v[j] * m[i][j];
    }
    return result;
}

template <typename T, unsigned int N>
inline Vec<T,N> operator*(Mat<T,N> m, Vec<T,N> v) {
    Vec<T,N> result;
    for(int j=0; j<N; ++j) {
        result[j] = m[0][j] * v[0];

        for(int i=1; i<N; ++i)
            result[j] += m[i][j] * v[i];
    }
    return result;
}

//////////////////////////////////////////////////////////////////////
// other generic matrix functions

// transpose (this'd be the place to add other matrix math operations)
template <typename T, unsigned int N>
inline Mat<T,N> transpose(Mat<T,N> m) {
    Mat<T,N> result;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            result[i][j] = m[j][i];
    return result;
}

// inverse of matrix, using Gaussian elimination
template <typename T, unsigned int N>
inline Mat<T,N> inverse(Mat<T,N> mat) {
    Mat<T,N> inv(1);
    // eliminate one row at a time
    for (int i=0; i<N; ++i) {
        // swap un-eliminated row with largest value
        for (int j=i+1; j<N; ++j) {
            if (std::abs(mat[j][i]) > std::abs(mat[i][i])) {
                std::swap(inv[i], inv[j]);
                std::swap(mat[i], mat[j]);
            }
        }
        
        // set first element to 1
        inv[i] = inv[i] / mat[i][i];
        mat[i] = mat[i] / mat[i][i];
        
        // eliminate from all other rows
        for (int j=0; j<N; ++j) {
            if (j == i) continue;
            inv[j] = inv[j] - inv[i] * mat[j][i];
            mat[j] = mat[j] - mat[i] * mat[j][i];
        }
    }
    
    return inv;
}
