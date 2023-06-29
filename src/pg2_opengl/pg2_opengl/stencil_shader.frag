#version 460 core

layout ( location = 0 ) out vec4 FragColor;

in vec3 fColor;

void main( void )
{	
	FragColor = vec4( fColor, 1.0f );
}
