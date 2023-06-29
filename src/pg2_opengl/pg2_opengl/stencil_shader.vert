#version 460 core

// vertex attributes
layout ( location = 0 ) in vec4 in_position_ms;

void main( void ) {
	gl_Position = in_position_ms;
}

