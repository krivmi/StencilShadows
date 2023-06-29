#version 460 core

// vertex attributes
layout ( location = 0 ) in vec4 in_position_ms;
layout ( location = 1 ) in vec3 in_normal_ms;
layout ( location = 2 ) in vec3 in_tangent_ms;
layout ( location = 3 ) in vec2 tex_coords;
layout ( location = 4 ) in vec3 in_color;
layout ( location = 5 ) in int index_material;

// uniform variables
uniform mat4 MN; // Model normal matrix 
uniform mat4 MVP; // Model View Projection Matrix
uniform vec3 view_from_position; // view position of camera
uniform mat4 mlp;
uniform mat4 M;

// output variables
out vec3 unified_normal_ws;
out vec3 position_ws;
out vec2 m_tex_coords;
out vec3 view_from_ws;
out vec3 m_color;
flat out uint out_index_material;

void main( void )
{
	// normal_ms to normal_ws
	vec4 tmp_normal = MN * vec4( in_normal_ms.xyz, 1.0f );
	unified_normal_ws = normalize( tmp_normal.xyz / tmp_normal.w );

	m_tex_coords = vec2( tex_coords.x, 1.0f - tex_coords.y);
	
	out_index_material = index_material;
	view_from_ws = view_from_position;
	m_color = in_color;

	// position_ms to position_ws
	vec4 tmp_position = M * vec4( in_position_ms.xyz, 1.0f );
	position_ws = tmp_position.xyz / tmp_position.w;

	gl_Position = MVP * in_position_ms;
}
