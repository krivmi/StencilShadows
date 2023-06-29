#version 460 core

layout ( location = 0 ) out vec4 FragColor;

// inputs
in vec2 texture_coords;

// uniforms
uniform sampler2D env_map;

void main( void )
{	
	vec3 env_col = texture( env_map, texture_coords ).rgb;
	FragColor = vec4( env_col , 1.0f );
}
