// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <map>
#include <memory>

#include "vector3.h"
#include "node.h"
#include "material.h"
#include "triangle.h"

struct Face3i
{
	Vertex3i vertices[3];	
	std::array<int, 3> adjacent_vertices = { -1, -1, -1 };
};

//typedef std::vector<Vertex3i> Patch; /* single patch: list of faces (triangles) indices */
typedef std::vector<Face3i> Patch; /* single patch: list of faces (triangles) indices */
typedef std::map<std::shared_ptr<Material>, Patch> Patches; /* patches grouped by materials */

/*! \class Mesh
\brief Mesh represents indexed geometry of a single object (from OBJ file) with possibly multiple materials assigned to individual faces.

\author Tomáš Fabián
\version 0.1
\date 2021
*/
class Mesh : public Node
{
public:
	Mesh( const std::string & name, const Matrix4x4 & transform = Matrix4x4() );

	Patches & patches();
	Patch & patch( std::shared_ptr<Material> material );
	VertexBuffer4f & vertex_buffer();

	/* get the list of all materials used in this mesh */
	std::vector<std::shared_ptr<Material>> materials() const;

	size_t push_back_position( const Vector3 position );
	size_t push_back_texture_coord( const Vector3 texture_coord );
	size_t push_back_normal( const Vector3 normal );

	/* number of triangles */
	size_t size() const;

	/* build (smoothed, averaged) tangents (TBN) on the basis of existing normals and texture coordinates */
	int BuildTangents();

	/* sets the adjacency of triangles */
	int BuildAdjacency();
		
	class iterator
	{
	public:		
		using difference_type = size_t ;
		using value_type = Triangle3i;
		using pointer = std::shared_ptr<Triangle3i>;
		using reference = Triangle3i &;
		using iterator_category = std::forward_iterator_tag;

		iterator( Mesh * mesh, const int state );

		iterator & operator++();
		iterator operator++( int );
		pointer operator*();		
		bool operator==( const iterator & rhs ) const;
		bool operator!=( const iterator & rhs ) const;

		void next();

		std::shared_ptr<Material> triangle_material()
		{
			return patches_iter_->first;
		}

	private:
		Mesh * mesh_{ nullptr };
		int state_{ -1 };			
	
		Patches::iterator patches_iter_;
		Patch::iterator patch_iter_;		
	};

	iterator begin();
	iterator end();	
	
private:
	Patches patches_;
	VertexBuffer4f vertex_buffer_;
};

#endif // MESH_H_
