// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef VERTEX_H_
#define VERTEX_H_

#include "vector3.h"

#include <vector>

/* indices of vertex attributes */
struct Vertex3i
{
	int position_index{ 0 }; /* vertex position index */
	int texture_coord_index{ 0 }; /* vertex texture index */
	int normal_index{ 0 }; /* normal and also tangent index */
};

/* vertex attributes */
struct Vertex4f
{
	Vector3 position; /* vertex position */
	Vector3 texture_coord; /* vertex texture coordinate */
	Vector3 normal; /* vertex normal */
	Vector3 tangent; /* vertex tangent */
};

/* structure of arrays of vertex attributes */
struct VertexBuffer4f
{
	std::vector<Vector3> positions;
	std::vector<Vector3> texture_coords;
	std::vector<Vector3> normals;
	std::vector<Vector3> tangents;

	/* reset array of tangents */
	int reset_tangents();

	/* renormalize tangents */
	int normalize_tangents();
};

#endif // VERTEX_H_
