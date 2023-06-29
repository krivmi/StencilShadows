#ifndef TUTORIALS_H_
#define TUTORIALS_H_

#include "vector3.h"
#include "pgmath.h"

bool check_gl( const GLenum error = glGetError() );
void glfw_callback( const int error, const char * description );
void GLAPIENTRY gl_callback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * user_param );
void framebuffer_resize_callback( GLFWwindow * window, int width, int height );
int LoadShader( const std::string & file_name, std::vector<char> & shader );
std::string LoadAsciiFile( const std::string & file_name );
GLint CheckShader( const GLenum shader );

int tutorial_1( const int width = 640, const int height = 480 );


#endif
