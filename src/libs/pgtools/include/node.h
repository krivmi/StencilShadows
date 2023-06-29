// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef NODE_H_
#define NODE_H_

#include <string>

#include "matrix4x4.h"

/* a single node of a scene graph */
class Node
{
public:
	Node( const std::string & name, const Matrix4x4 & transform );

private:
	std::string name_; /* node name */
	Matrix4x4 transform_; /* model matrix (ms->ws)*/	
};

#endif // NODE_H_
