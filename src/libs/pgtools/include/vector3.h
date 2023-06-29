// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef VECTOR3_H_
#define VECTOR3_H_

/*! \class Vector3
\brief A vector in three-dimensional cartesian coordinates.

Implementace tøísložkového reálného vektoru podporující základní
matematické operace.

\note
Vektor se považuje za sloupcový, pøestože je v komentáøích pro jednoduchost
uvádìn jako øádkový.

\code{.cpp}
Vector3 v = Vector3( 2.0f, 4.5f, 7.8f );
v.Normalize();
\endcode

\author Tomáš Fabián
\version 0.99
\date 2007-2021
*/
class Vector3
{
public:
	union	// anonymní unie
	{
		struct
		{
			float x; /*!< První složka vektoru. */
			float y; /*!< Druhá složka vektoru. */
			float z; /*!< Tøetí složka vektoru. */
		};

		struct
		{
			float r; /*!< První složka vektoru. */
			float s; /*!< Druhá složka vektoru. */
			float t; /*!< Tøetí složka vektoru. */
		};

		std::array<float, 3> data; /*!< Pole složek vektoru. */
	};

	//! Výchozí konstruktor.
	/*!
	Inicializuje všechny složky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) { }

	//! Obecný konstruktor.
	/*!
	Inicializuje složky vektoru podle zadaných hodnot parametrù,
	\f$\mathbf{v}=(x,y,z)\f$.

	\param x první složka vektoru.
	\param y druhá složka vektoru.
	\param z tøetí složka vektoru.
	*/
	Vector3( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { }

	//! Konstruktor z pole.
	/*!
	Inicializuje složky vektoru podle zadaných hodnot pole,

	\param v ukazatel na první složka vektoru.	
	*/
	Vector3( const float * v );

	/* builds unit vector in cartesian coordinates from the given spherical coordinates where theta is polar angle <0, pi> and phi is azimuthal angle <0, 2pi) */
	Vector3( const float theta, const float phi );

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
	Vector3 CrossProduct( const Vector3 & v ) const;	

	/* projects vector v orhogonally onto the line spanned by this vector */
	Vector3 Project( const Vector3 & v ) const;

	Vector3 Abs() const;

	Vector3 Max( const float a = 0 ) const;

	float max_value() const;

	float min_value() const;

	//! Skalární souèin.
	/*!		
	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_x + \mathbf{u}_y \mathbf{v}_y + \mathbf{u}_z \mathbf{v}_z)\f$.
	*/
	float DotProduct( const Vector3 & v ) const;

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

	/*
* return reflected vector v around the normal n
* in 3D, reflection occurs in the plane defined by input vectors v and n, thus the reflected (output) vector lies in the same plane
*/
	Vector3 reflect( const Vector3 & n ) const;

	// https://math.stackexchange.com/questions/2373703/generate-an-orthonormal-basis-belonging-to-a-plan-defined-by-it-normal-vector
	/* builds an orthogonal vector to this vector */
	Vector3 orthogonal() const;

	Vector3 half_way( const Vector3 & b ) const;	

	/* this vector is in RS and output vector is in WS, normal vector is in WS */
	Vector3 transform_rs2ws( const Vector3 & normal ) const;

	/* this vector is in WS and output vector is in RS, normal vector is in WS */
	Vector3 transform_ws2rs( const Vector3 & normal ) const;

	// --- operátory ------

	friend Vector3 operator-( const Vector3 & v );

	friend Vector3 operator+( const Vector3 & u, const Vector3 & v );
	friend Vector3 operator-( const Vector3 & u, const Vector3 & v );

	friend Vector3 operator*( const Vector3 & v, const float a );	
	friend Vector3 operator*( const float a, const Vector3 & v );
	friend Vector3 operator*( const Vector3 & u, const Vector3 & v );

	friend Vector3 operator/( const Vector3 & v, const float a );

	friend void operator+=( Vector3 & u, const Vector3 & v );
	friend void operator-=( Vector3 & u, const Vector3 & v );
	friend void operator*=( Vector3 & v, const float a );
	friend void operator/=( Vector3 & v, const float a );		
	friend bool operator==( const Vector3 & u, const Vector3 & v );
};

static_assert( sizeof( Vector3 ) == 3 * sizeof( float ), "Vector3 size is not correct" );

/*namespace YAML
{
	template<> struct convert<Vector3>
	{
		static Node encode( const Vector3 & rhs )
		{
			Node node;
			node.push_back( rhs.x );
			node.push_back( rhs.y );
			node.push_back( rhs.z );

			return node;
		}

		static bool decode( const Node & node, Vector3 & rhs )
		{
			if ( !node.IsSequence() || node.size() != 3 )
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};
}*/

#endif // VECTOR3_H_
