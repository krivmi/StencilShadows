// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <optional>
#include <array>

#include "vertex.h"

/*! \class AbstractTriangle
\brief Abstract triangle determined by three points.

\author Tomáš Fabián
\version 0.1
\date 2021
*/
class AbstractTriangle
{
public:
	/* returns position of the i-th vertex */
	virtual Vector3 & position( const int i ) = 0;	

	/* returns constant position of the i-th vertex */
	virtual const Vector3 & position( const int i ) const = 0;	

	/* returns normal vector of the i-th vertex */
	virtual Vector3 & normal( const int i ) = 0;

	/* returns constant normal vector of the i-th vertex */
	virtual const Vector3 & normal( const int i ) const = 0;

	/* checks if the triangle is degenerate */
	bool is_degenerate() const;

	/* returns triangle area */
	float area() const;
	 
	/* uniform sampling of the triangle with probability 1/Area, see http://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations.html */
	Vector3 random_point( const float ksi1, const float ksi2 ) const;

	/* returns interpolated normal at the given 3D point inside the triangle */
	Vector3 normal( const Vector3 & p ) const;
};

/*! \class Triangle4f
\brief Local copy of a triangle determined by three vertices and their attributes.

TODO consider removal

\author Tomáš Fabián
\version 0.1
\date 2021
*/
class Triangle4f : public AbstractTriangle
{
public:
	Triangle4f( const Vertex4f & v0, const Vertex4f & v1, const Vertex4f & v2 )
	{
		vertices_[0] = v0;
		vertices_[1] = v1;
		vertices_[2] = v2;
	}

	Triangle4f( const Vertex4f & v0, const Vertex4f & v1, const Vertex4f & v2,
		const std::optional<Vector3> & a0, const std::optional<Vector3> & a1, const std::optional<Vector3> & a2 )
	{
		vertices_[0] = v0;
		vertices_[1] = v1;
		vertices_[2] = v2;

		adjacent_vertices_[0] = a0;
		adjacent_vertices_[1] = a1;
		adjacent_vertices_[2] = a2;
	}

	Vector3 & position( const int i )
	{
		return vertices_[i].position;
	}

	const Vector3 & position( const int i ) const
	{
		return vertices_[i].position;
	}

	Vector3 & texture_coord( const int i )
	{
		return vertices_[i].texture_coord;
	}

	const Vector3 & texture_coord( const int i ) const
	{
		return vertices_[i].texture_coord;
	}

	using AbstractTriangle::normal; // see https://stackoverflow.com/questions/15556535/calling-base-classs-overloaded-method

	Vector3 & normal( const int i )
	{
		return vertices_[i].normal;
	}

	const Vector3 & normal( const int i ) const
	{
		return vertices_[i].normal;
	}

	Vector3 & tangent( const int i )
	{
		return vertices_[i].tangent;
	}

	const Vector3 & tangent( const int i ) const
	{
		return vertices_[i].tangent;
	}

	Vector3 bitangent( const int i ) const
	{
		return vertices_[i].normal.CrossProduct( vertices_[i].tangent );
	}

	Vertex4f & vertex( const int i )
	{
		return vertices_[i];
	}

	const Vertex4f & vertex( const int i ) const
	{
		return vertices_[i];
	}

	/* position of the vertex adjacent to the i-th edge or NaN */
	std::optional<Vector3> adjacent_vertex_position( const int i ) const
	{
		return adjacent_vertices_[i];
	}

private:
	std::array<Vertex4f, 3> vertices_;
	std::array<std::optional<Vector3>, 3> adjacent_vertices_;
};

/*! \class Triangle3i
\brief Indexed triangle determined by three indices into the given lists of attributes.

\author Tomáš Fabián
\version 0.1
\date 2021
*/
class Triangle3i : public AbstractTriangle
{
public:
	Triangle3i( const Vertex3i & v0, const Vertex3i & v1, const Vertex3i & v2,
		std::array<int, 3> & adjacent_vertices,	VertexBuffer4f & vertex_buffer ) :
		adjacent_vertices_( adjacent_vertices ), vertex_buffer_( vertex_buffer )
	{
		vertices_[0] = v0;
		vertices_[1] = v1;
		vertices_[2] = v2;
	}

	/* translates the local index of triangle vertex into the global index of position */
	int position_index( const int i ) const
	{
		return vertices_[i].position_index;
	}

	Vector3 & position( const int i )
	{
		return vertex_buffer_.positions[position_index( i )];
	}

	const Vector3 & position( const int i ) const
	{
		return vertex_buffer_.positions[position_index( i )];
	}

	int texture_coord_index( const int i ) const
	{
		return vertices_[i].texture_coord_index;
	}

	Vector3 & texture_coord( const int i )
	{		
		return vertex_buffer_.texture_coords[texture_coord_index( i )];
	}

	Vector3 & texture_coord( const int i ) const
	{
		return vertex_buffer_.texture_coords[texture_coord_index( i )];
	}

	int normal_index( const int i ) const
	{
		return vertices_[i].normal_index;
	}

	Vector3 & normal( const int i )
	{	
		return vertex_buffer_.normals[normal_index( i )];
	}

	Vector3 & normal( const int i ) const
	{
		return vertex_buffer_.normals[normal_index( i )];
	}

	int tangent_index( const int i ) const
	{
		return normal_index( i );
	}

	Vector3 & tangent( const int i )
	{	
		return vertex_buffer_.tangents[tangent_index( i )]; // note that normal and tangent share the same index
	}

	Vector3 & tangent( const int i ) const
	{
		return vertex_buffer_.tangents[tangent_index( i )]; // note that normal and tangent share the same index
	}

	Vector3 bitangent( const int i ) const
	{
		return normal( i ).CrossProduct( tangent( i ) );
	}

	/* get i-th vertex of the triangle */
	Vertex4f vertex( const int i )
	{
		return Vertex4f( { position( i ), texture_coord( i ), normal( i ), tangent( i ) } );
	}

	/* position of the vertex adjacent to the i-th edge or NaN */
	std::optional<Vector3> adjacent_vertex_position( const int i ) const
	{
		if ( adjacent_vertices_[i] < 0 )
		{
			return {}; // adjacent vertex does not exist
		}

		return vertex_buffer_.positions[adjacent_vertices_[i]];
	}

	/* cast indexed triangle to regular triangle */
	explicit operator Triangle4f()
	{
		return Triangle4f( vertex( 0 ), vertex( 1 ), vertex( 2 ), adjacent_vertex_position( 0 ), adjacent_vertex_position( 1 ), adjacent_vertex_position( 2 ) );
	}	

	/* updates shared tangents */
	int UpdateTangents();	

	/* sets edge adjacency */
	int set_adjacency( const int edge, const int vertex );

	/* gets edge adjacency */
	int adjacency( const int edge ) const;

protected:
	Vector3 Tangent( const Vector3 & t, const Vector3 & b, const Vector3 & n ) const;

private:	
	std::array<Vertex3i, 3> vertices_;
	std::array<int, 3> & adjacent_vertices_; /* indices to adjacent vertices position array, TODO consider shared ptr instead of ref */

	VertexBuffer4f & vertex_buffer_; // TODO consider shared ptr instead of ref
};

#endif // TRIANGLE_H_
