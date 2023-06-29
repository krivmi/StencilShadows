#version 460 core
// vertex attributes
layout ( location = 0 ) in vec4 in_position_ms;
layout ( location = 3 ) in vec2 in_texture_coords;

// uniform variables
uniform mat4 MVP; 

// output variables
out vec2 texture_coords;

void main( void )
{
	texture_coords = vec2(1.0f - in_texture_coords.x, 1.0f - in_texture_coords.y);
	gl_Position = MVP * vec4(in_position_ms.xyz, 0.0f);
}