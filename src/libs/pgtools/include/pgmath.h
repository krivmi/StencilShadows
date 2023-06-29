// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef PG_MATH_H_
#define PG_MATH_H_

#define _USE_MATH_DEFINES
#include <math.h>

template <class T> inline T min( const T a, const T b )
{
	return ( a <= b ) ? a : b;
}

template <class T> inline T max( const T a, const T b )
{
	return ( a >= b ) ? a : b;
}

/* returns the square of the specified value x */
template <class T> inline T sqr( const T x )
{
	return x * x;
}

/* convert degrees to radians */
inline float deg2rad( const float x )
{
	return x * float( M_PI ) / 180.0f;
}

/* update the mean value by new sample providing the total number of samples */
template <class T> inline void update( T & mean, const T new_sample, const int samples_count )
{
	mean = T( ( mean * ( samples_count - T( 1 ) ) + new_sample ) / samples_count );
}

/* restrict value x to the given range <a=0, b=1> */
template <class T> inline float clamp( const T x, const T a = (T)0, const T b = (T)1 )
{
	return min( max( x, a ), b );
}

/* the Heaviside step function */
float Heaviside( const float x );

float gamma_quot( const float a, const float b );

/**
* Returns the full (non-normalised) incomplete beta function
* https://www.wolframalpha.com/input/?i=Beta%280.1%2C+9%2C+9%29
*/
float ibeta( const float a, const float b, const float x, const int n = 16 );

/* simple hash function */
unsigned long long QuickHash( const unsigned char * data, const size_t length, unsigned long long mix = 0 );

#endif // PG_MATH_H
