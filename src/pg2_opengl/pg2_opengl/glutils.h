#ifndef GL_UTILS_H_
#define GL_UTILS_H_

void SetInt(const GLuint program, GLint value, const char* int_name);
void SetFloat( const GLuint program, GLint value, const char * int_name );
void SetSampler( const GLuint program, GLenum texture_unit, const char * sampler_name );
void SetMatrix4x4( const GLuint program, const GLfloat * data, const char * matrix_name );
void SetVector3( const GLuint program, const GLfloat * data, const char * vector_name );
void SetVector2( const GLuint program, const GLfloat * data, const char * vector_name );

#endif
