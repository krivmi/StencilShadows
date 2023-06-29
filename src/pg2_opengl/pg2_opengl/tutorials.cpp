#include "pch.h"
#include "tutorials.h"
#include "camera.h"
#include "rasterizer.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"
#include "color.h"
#include "texture.h"
#include "objloader.h"

/* create a window and initialize OpenGL context */
int tutorial_1( const int width, const int height )
{
	Rasterizer rasterizer = Rasterizer();
	rasterizer.initOpenGl(width, height);

	rasterizer.loadMesh("../../../data/geosphere.obj", "map");
	rasterizer.loadMesh_triangles("../../../data/shadow_volume_test.obj");
	//rasterizer.loadMesh_triangles("../../../data/deer2.obj");
	//rasterizer.loadMesh_triangles("../../../data/test.obj");
	rasterizer.initShaders();

	rasterizer.initCamera(width, height, deg2rad(45.0), Vector3(0.374, 7.928, 5.02), Vector3(0, 0, 0)); // (x, z, y)
	rasterizer.initLight(Vector3(50.0f, 0.0f, 70.0f), 1.0f, true);
	
	rasterizer.initSurface();
	rasterizer.initSurfaceEnvMap();
	rasterizer.initSurfaceTriangles();

	rasterizer.InitEnvMap("../../../data/pref_env_2048.exr");
	rasterizer.SetEnvMap();

	rasterizer.mainLoop();

	return 0;
}

/* glfw callback */
void glfw_callback(const int error, const char* description)
{
	printf("GLFW Error (%d): %s\n", error, description);
}

/* OpenGL messaging callback */
void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
	printf("GL %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "Error" : "Message"),
		type, severity, message);
}

/* invoked when window is resized */
void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
std::string LoadAsciiFile(const std::string& file_name)
{
	std::ifstream file(file_name, std::ios::in);

	if (file) {
		return (std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
	}
	else { return ""; }
}


