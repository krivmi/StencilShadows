// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_

#include <string>
#include <map>

#include "mesh.h"
#include "material.h"

typedef std::map<std::string, std::shared_ptr<Node>> SceneGraph; /* definition of used scene graph structure */
typedef std::map<std::string, std::shared_ptr<Material>> MaterialLibrary; /* definition of used material library structure */

/* loads meshes and materials from referred OBJ a MTL files */
int LoadOBJ( const std::string & file_name, SceneGraph & scene, MaterialLibrary & materials, const bool build_adjacency = true );

#endif // OBJ_LOADER_H_
