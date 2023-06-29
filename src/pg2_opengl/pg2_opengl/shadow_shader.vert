#version 460 core
// vertex attributes
layout ( location = 0 ) in vec4 in_position_ms;

// uniform variables
uniform mat4 mlp; // Projection (P_l)*Light (V_l)*Model (M) matrix

void main( void )
{
	gl_Position = mlp * in_position_ms;
}