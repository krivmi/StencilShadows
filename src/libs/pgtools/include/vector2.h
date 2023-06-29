// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <array>

#include "vector3.h"

/*! \struct Vector2
\brief Dvourozmìrný (2D) vektor.

Implementace dvousložkového reálného vektoru podporující základní matematické operace.

\note
Vektor se považuje za sloupcový, pøestože je v komentáøích pro jednoduchost uvádìn jako øádkový.

\code{.cpp}
Vector2 v = Vector2( 2.0f, -7.8f );
v.Normalize();
\endcode

\author Tomáš Fabián
\version 0.95
\date 2007-2021
*/
class Vector2
{
public:
	union	// anonymní unie
	{
		struct
		{
			float x; /*!< První složka vektoru. */
			float y; /*!< Druhá složka vektoru. */			
		};

		struct
		{
			float u; /*!< První složka vektoru. */
			float v; /*!< Druhá složka vektoru. */
		};

		std::array<float, 2> data; /*!< Pole složek vektoru. */		
	};

	//! Výchozí konstruktor.
	/*!
	Inicializuje všechny složky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector2() : x( 0 ), y( 0 ) { }	

	//! Obecný konstruktor.
	/*!
	Inicializuje složky vektoru podle zadaných hodnot parametrù,
	\f$\mathbf{v}=(x,y,z)\f$.

	\param x první složka vektoru.
	\param y druhá složka vektoru.	
	*/
	Vector2( const float x, const float y ) : x( x ), y( y ) { }

	//! Konstruktor z pole.
	/*!
	Inicializuje složky vektoru podle zadaných hodnot pole,

	\param v ukazatel na první složka vektoru.	
	*/
	Vector2( const float * v );

	//! L2-norma vektoru.
	/*!
	\return x Hodnotu \f$\mathbf{||v||}=\sqrt{x^2+y^2+z^2}\f$.
	*/
	float L2Norm() const;

	//! Druhá mocnina L2-normy vektoru.
	/*!
	\return Hodnotu \f$\mathbf{||v||^2}=x^2+y^2+z^2\f$.
	*/
	float SqrL2Norm() const;

	//! Normalizace vektoru.
	/*!
	Po provedení operace bude mít vektor jednotkovou délku.
	*/
	float Normalize();

	//! Vektorový souèin.
	/*!
	\param v vektor \f$\mathbf{v}\f$.

	\return Vektor \f$(\mathbf{u}_x \mathbf{v}_z - \mathbf{u}_z \mathbf{v}_y,
	\mathbf{u}_z \mathbf{v}_x - \mathbf{u}_x \mathbf{v}_z,
	\mathbf{u}_x \mathbf{v}_y - \mathbf{u}_y \mathbf{v}_x)\f$.
	*/
	Vector3 CrossProduct( const Vector2 & v ) const;	

	/* projects vector v orhogonally onto the line spanned by this vector */
	Vector2 Project( const Vector2 & v ) const;

	Vector2 Abs() const;

	Vector2 Max( const float a = 0 ) const;

	float max_value() const;

	float min_value() const;

	//! Skalární souèin.
	/*!		
	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_x + \mathbf{u}_y \mathbf{v}_y + \mathbf{u}_z \mathbf{v}_z)\f$.
	*/
	float DotProduct( const Vector2 & v ) const;

	//! Rotace.
	/*!		
	\return Vektor orotovaný kolem poèátku o \f$\phi\f$ radiánù v kladném smyslu.
	*/
	//Vector2 Rotate( const float phi );

	//! Index nejvìtší složky vektoru.
	/*!
	\param absolute_value index bude urèen podle absolutní hodnoty složky

	\return Index nejvìtší složky vektoru.
	*/
	char LargestComponent( const bool absolute_value = false );	

	void Print() const;

	// --- operátory ------

	friend Vector2 operator-( const Vector2 & v );

	friend Vector2 operator+( const Vector2 & u, const Vector2 & v );
	friend Vector2 operator-( const Vector2 & u, const Vector2 & v );

	friend Vector2 operator*( const Vector2 & v, const float a );	
	friend Vector2 operator*( const float a, const Vector2 & v );
	friend Vector2 operator*( const Vector2 & u, const Vector2 & v );

	friend Vector2 operator/( const Vector2 & v, const float a );

	friend void operator+=( Vector2 & u, const Vector2 & v );
	friend void operator-=( Vector2 & u, const Vector2 & v );
	friend void operator*=( Vector2 & v, const float a );
	friend void operator/=( Vector2 & v, const float a );		
	friend bool operator==( const Vector2 & u, const Vector2 & v );
};

static_assert( sizeof( Vector2 ) == 2 * sizeof( float ), "Vector3 size is not correct" );

#endif // VECTOR2_H_
