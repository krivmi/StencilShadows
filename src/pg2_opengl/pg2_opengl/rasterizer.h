#include "pch.h"
#include "tutorials.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"
#include "camera.h"
#include "light.h"
#include "vector2.h"
#include "objloader.h"

struct Vertex
{
	Vector3 position; /* vertex position */
	Vector3 normal; /* vertex normal */
	Vector3 tangent; /* vertex tangent */
	Vector2 texture_coord; /* vertex texture coordinate */
	Vector3 color; /* vertex color */
	int material_index {0}; /* material index */
};

struct TriangleWithAdjacency
{
	std::array<Vertex, 6> vertices;
};

class Rasterizer{
public:

	Rasterizer();

	int initOpenGl(int width, int height);
	void initSurface();
	void initSurfaceEnvMap();
	void initSurfaceTriangles();
	void initShaders();
	void initCamera(int width, int height, float FOV_y, Vector3 view_from, Vector3 view_at);
	void initLight(Vector3 position, float intensity, bool move = true);

	void loadMesh(const std::string& file_name, const std::string model);
	void loadMesh_triangles(const std::string& file_name);
	int loadShader(const std::string& file_name, std::vector<char>& shader);
	GLint checkShader(const GLenum shader);
	
	int InitEnvMap(const std::string& file_name);
	int SetEnvMap();

	int mainLoop();
private:
	Camera camera;
	Light light;

	GLFWwindow* window;
	GLuint shader_program;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint geometry_shader_stencil;

	GLuint vertex_shader_env;
	GLuint fragment_shader_env;
	GLuint env_program{ 0 };

	GLuint vertex_shader_stencil;
	GLuint fragment_shader_stencil;
	GLuint stencil_program{ 0 };

	GLuint vbo_env{ 0 };
	GLuint vao_env{ 0 };

	GLuint vbo{ 0 };
	GLuint vao{ 0 };

	GLuint tex_irradiance_map{ 0 };
	GLuint tex_normal_map{ 0 };
	GLuint tex_albedo_map{ 0 };
	GLuint tex_pref_env_map{ 0 };
	GLuint tex_env_map{ 0 };
	GLuint tex_BRDF_map{ 0 };
	GLuint tex_RMA_map{ 0 };

	GLuint vertex_shader_shadow;
	GLuint fragment_shader_shadow;
	GLuint shadow_program_{ 0 }; // collection of shadow mapping shaders for shadow mapping and depth pass


	bool map_loaded{false};

	std::vector<Vertex> loadedVerticesMap;
	std::vector<Vertex> loadedVertices;
	std::vector<TriangleWithAdjacency> loaded_triangles;

	MaterialLibrary materials_;
};

