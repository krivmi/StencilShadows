// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef MATRIX_3X3_H_
#define MATRIX_3X3_H_

#include <array>

#include "vector3.h"

/*! \class Matrix3x3
\brief Real matrix 3x3 arranged in row-major order.

\author Tomáš Fabián
\version 1.1
\date 2018-2021
*/
class Matrix3x3
{
public:
	union
	{
		/* Row-major
		m00 m01 m02
		m10 m11 m12
		m20 m21 m22
		*/

		struct
		{
			float m00;
			float m01;
			float m02;

			float m10;
			float m11;
			float m12;

			float m20;
			float m21;
			float m22;
		};

		std::array<float, 3 * 3> data;
	};

	/* construct a new identity matrix */
	Matrix3x3();

	/* construct a new matrix from nine scalar values */
	Matrix3x3( const float m00, const float m01, const float m02,
		const float m10, const float m11, const float m12,
		const float m20, const float m21, const float m22 );

	/* construct a new matrix from three column (basis) vectors */
	Matrix3x3( const Vector3 basis_x, const Vector3 basis_y, const Vector3 basis_z );		

	/* build TBN matrix based on the given normal vector */
	//Matrix3x3( const Vector3 & n );

	/* return matrix transpose which flips a matrix over its diagonal */
	Matrix3x3 Transpose() const;
	
	/* set matrix element */
	void set( const int row, const int column, const float value );	

	/* set matrix element */
	float & operator() ( const int row, const int column );

	/* get matrix element */
	float get( const int row, const int column ) const;

	/* get matrix element */
	float operator() ( const int row, const int column ) const;	

	/* get matrix column */
	Vector3 column( const int column ) const;

	/* set matrix column with the given vector values */
	void set_column( const int column, const Vector3 & v );
	
	/* get matrix row */
	Vector3 row( const int row ) const;

	/* set matrix row with the given vector values */
	void set_row( const int row, const Vector3 & v );
		
	/* multiply matrix by a vector */
	friend Vector3 operator*( const Matrix3x3 & a, const Vector3 & b );

	/* multiply matrix by a second matrix */
	friend Matrix3x3 operator*( const Matrix3x3 & a, const Matrix3x3 & b );	
};

static_assert( sizeof( Matrix3x3 ) == 3 * 3 * sizeof( float ), "Matrix3x3 size is not correct" );

typedef Matrix3x3 Matrix3;

#endif // MATRIX_3X3_H_
