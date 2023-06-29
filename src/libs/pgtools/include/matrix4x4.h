// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef MATRIX_4X4_H_
#define MATRIX_4X4_H_

#include <string>
#include <assert.h>

#include "matrix3x3.h"

/*! \class Matrix4x4
\brief Reálná matice 4x4 uspoøádaná po øádcích.

\author Tomáš Fabián
\version 1.0
\date 2015
*/
class Matrix4x4
{
public:
	//! Výchozí konstruktor.
	/*!
	Inicializace na matici identity.
	*/
	Matrix4x4();

	//! Výchozí konstruktor.
	/*!
	Inicializace matice zadanými hodnotami. První index oznaèuje øádek, druhý index patøí sloupci.
	*/
	Matrix4x4( const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33 );

	Matrix4x4( const Vector3 & axis_x, const Vector3 & axis_y, const Vector3 & axis_z,
		const Vector3 & view_from );

	//! Transpozice matice.
	/*!
	Provede traspozici matice vzájemnou výmìnou øádkù a sloupcù.
	*/
	void Transpose();

	//! Euklidovská inverze matice.
	/*!
	Provede inverzi matice. Výsledek je správný jen za pøedpokladu, že se jedná o matici
	pøedstavující Euklidovské transformace zahrnující translaci,
	rotaci a reflexi. Taková transformace mìní pouze orientaci a pozici objektù, úhly a
	délky zùstávají zachovány.
	*/
	void EuclideanInverse();

	//! Euklidovská inverze matice.
	/*!
	Stejná funkcionalita jako u \a EuclideanInverse().

	\return Výsledek inverze matice \a m.
	*/
	static Matrix4x4 EuclideanInverse( Matrix4x4 m );

	//! Nastaví zadaný prvek matice na novou hodnotu.
	/*!
	\param row øádek matice.
	\param column sloupec matice.
	\param value nová hodnota prvku matice.
	*/
	void set( const int row, const int column, const float value );

	//! Vrátí zadaný prvek matice.
	/*!
	\param row øádek matice.
	\param column sloupec matice.
	\return Požadovaný prvek matice.
	*/
	float get( const int row, const int column ) const;

	//! Ukazatel na prvky matice.
	/*!
	\return Ukazatel na prvky matice.
	*/
	float * data();

	Matrix3x3 so3() const;
	Vector3 tr3() const;
	void so3(const Matrix3x3 & m);
	void tr3( const Vector3 & v );		

	std::string toString() const;	

	bool operator==( const Matrix4x4 & m ) const;

	//friend Vector4 operator*( const Matrix4x4 & a, const Vector4 & b );
	friend Matrix4x4 operator*( const Matrix4x4 & a, const Matrix4x4 & b );

private:
#pragma warning( push )
#pragma warning ( disable : 4201 )
	union
	{
		/* Row-major
		m00 m01 m02 m03
		m10 m11 m12 m13
		m20 m21 m22 m23
		m30 m31 m32 m33
		*/

		struct
		{
			float m00_;
			float m01_;
			float m02_;
			float m03_;

			float m10_;
			float m11_;
			float m12_;
			float m13_;

			float m20_;
			float m21_;
			float m22_;
			float m23_;

			float m30_;
			float m31_;
			float m32_;
			float m33_;
		};

		std::array<float, 4 * 4> data_;
	};
#pragma warning( pop ) 
};

typedef Matrix4x4 Matrix4;

template<class T>
class Matrix
{
public:
	Matrix( const int rows, const int cols )
	{
		assert( ( rows > 0 ) && ( cols > 0 ) );
		
		data_ = new T[rows*cols];
		rows_ = rows;
		cols_ = cols;
	}

	~Matrix()
	{
		SAFE_DELETE_ARRAY( data_ );
		rows_ = 0;
		cols_ = 0;
	}

	T * operator()( const int row )
	{
		assert( ( row >= 0 ) && ( row < rows_ ) );

		return &data_[row * cols_];
	}

	T & operator()( const int row, const int col )
	{
		assert( ( row >= 0 ) && ( row < rows_ ) );
		assert( ( col >= 0 ) && ( col < cols_ ) );

		return data_[row * cols_ + col];
	}

private:
	T * data_;
	int rows_;
	int cols_;
};

#endif // MATRIX_4X4_H_
