#version 460 core

layout ( location = 0 ) out vec4 FragColor;

void main( void )
{
	// by default the z component of gl_FragCoord is assigned to gl_FragDepth so we don't have to do anything here
	//gl_FragDepth = gl_FragCoord.z;
	//FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
}