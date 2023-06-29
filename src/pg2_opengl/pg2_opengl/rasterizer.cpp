#include "pch.h"
#include "rasterizer.h"
#include "camera.h"
#include "light.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"
#include "color.h"
#include "texture.h"
#include "objloader.h"

Rasterizer::Rasterizer() {}

int Rasterizer::mainLoop() {
	float counter = 0.0f;

	glPointSize(1.0f);
	glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0); // clear stencil buffer
	glStencilMask(0xFF); // enable all bits for writing to the stencil 
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // GL_LESS for normal pass, GL_LEQUAL for displaying points in infinity
	//glDepthRange(0.0f, 1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, camera.getWidth(), camera.getHeight());

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		counter += 0.05f;
		//printf("%f\n", counter);
		
		camera.Update();
		camera.Inputs(window);

		light.Update(counter);

		Vector3 view_from = camera.getViewFrom();
		std::vector<float> view_from_v = { view_from.x, view_from.y, view_from.z };
		std::vector<float> light_position_ws = { light.position.x, light.position.y, light.position.z };
		
		glStencilMask(0xFF);
		glDepthMask(GL_TRUE);
		glDisable(GL_STENCIL_TEST);

		// clear the scene
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// --- DEPTH PASS ---
		glUseProgram(shadow_program_);

		SetMatrix4x4(shadow_program_, camera.MVP.data(), "mlp");

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES_ADJACENCY, 0, loaded_triangles.size() * 6);
		glBindVertexArray(0);

		if (map_loaded) {

			// --- ENVIRONMENT PASS ---
			glUseProgram(env_program);

			glEnable(GL_DEPTH_CLAMP);
			glDepthFunc(GL_LEQUAL);
			glDisable(GL_CULL_FACE);

			SetMatrix4x4(env_program, camera.MVP.data(), "MVP");
			SetEnvMap();

			glBindVertexArray(vao_env);
			glDrawArrays(GL_TRIANGLES, 0, loadedVerticesMap.size());
			glBindVertexArray(0);

			glDisable(GL_DEPTH_CLAMP);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
		}
		
		// --- SHADOW PASS ---
		glUseProgram(stencil_program);
		
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE); 
		glEnable(GL_DEPTH_CLAMP); 
		glDisable(GL_CULL_FACE); 
		glFrontFace(GL_CCW);
		glEnable(GL_STENCIL_TEST);
		glDepthFunc(GL_LESS);
		glStencilMask(0xFF);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP); 
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP); 
		glStencilFunc(GL_ALWAYS, 0, 0xFF); 
		
		SetMatrix4x4(stencil_program, camera.MVP.data(), "MVP");
		SetVector3(stencil_program, light_position_ws.data(), "light_position");

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES_ADJACENCY, 0, loaded_triangles.size()* 6);
		glBindVertexArray(0);
		
		// --- LIGHTNING PASS ---
		glUseProgram(shader_program);
		
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glDisable(GL_DEPTH_CLAMP);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE); 
		glCullFace(GL_BACK);
		glEnable(GL_STENCIL_TEST);
		glStencilMask(0);
		glStencilOpSeparate(GL_FRONT_AND_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0, 0xFF);

		//amb_int = 1.0f;

		SetMatrix4x4(shader_program, camera.MVP.data(), "MVP");
		SetMatrix4x4(shader_program, camera.MN.data(), "MN");
		SetVector3(shader_program, view_from_v.data(), "view_from_position");
		SetMatrix4x4(shader_program, camera.M.data(), "M");
		//SetFloat(shader_program, amb_int, "amb_int");
		SetVector3(shader_program, light_position_ws.data(), "light_position");

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES_ADJACENCY, 0, loaded_triangles.size() * 6);
		glBindVertexArray(0);
		
		// -- AMBIENT PASS --
		glUseProgram(shader_program);
		
		//amb_int = 1.0f;
		
		//SetFloat(shader_program, amb_int, "amb_int");

		glDisable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES_ADJACENCY, 0, loaded_triangles.size() * 6);
		glBindVertexArray(0);
		
		
		glfwSwapBuffers(window); 
		glfwPollEvents(); 
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader_env);
	glDeleteShader(fragment_shader_env);
	glDeleteShader(vertex_shader_stencil);
	glDeleteShader(geometry_shader_stencil);
	glDeleteShader(fragment_shader_stencil);
	glDeleteShader(vertex_shader_stencil);
	glDeleteShader(vertex_shader_shadow);
	glDeleteShader(fragment_shader_shadow);

	glDeleteProgram(shader_program);
	glDeleteProgram(shadow_program_);
	glDeleteProgram(env_program);
	glDeleteProgram(stencil_program);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vbo_env);
	glDeleteVertexArrays(1, &vao_env);

	glfwTerminate();

	return EXIT_SUCCESS;
}
int Rasterizer::InitEnvMap(const std::string& file_name)
{
	Texture3f env_map = Texture3f(file_name);

	glGenTextures(1, &tex_env_map);
	glBindTexture(GL_TEXTURE_2D, tex_env_map);
	if (glIsTexture(tex_env_map))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// for HDR images use GL_RGB32F or GL_RGB16F as internal format !!!
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
			env_map.width(), env_map.height(), 0,
			GL_RGB, GL_FLOAT, env_map.data());
		//glGenerateMipmap( GL_TEXTURE_2D );
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return S_OK;
}
int Rasterizer::SetEnvMap()
{
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, tex_env_map);

	SetSampler(env_program, 7, "env_map");

	return S_OK;
}
int Rasterizer::initOpenGl(int width, int height) {
	glfwSetErrorCallback(glfw_callback);

	if (!glfwInit())
	{
		return(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // verze
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // verze
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile
	glfwWindowHint(GLFW_SAMPLES, 8); // anti-aliasing (8 samplù)
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // zmìna velikosti okna za bìhu
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); // na back buffer kreslím, front vidím (pro plynulost)

	window = glfwCreateWindow(width, height, "OpenGL - stencil shadows", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // když se zmìní velikost okna
	glfwMakeContextCurrent(window); // v OPENGL mùžu volat jen z vlákna, ve které jsme to vytvoøili
	// nastavím domovský current kontext

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // naètu všechny funkce OpenGL
	{
		if (!gladLoadGL())
		{
			return EXIT_FAILURE;
		}
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_callback, nullptr); // v OpenGL je složité debugovat 
	// když vznikne chba, vrátí se do konzole ten error (errory jsou v referenèní pøíruèce)

	printf("OpenGL %s, ", glGetString(GL_VERSION));
	printf("%s", glGetString(GL_RENDERER));
	printf(" (%s)\n", glGetString(GL_VENDOR));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_MULTISAMPLE);

	// map from the range of NDC coordinates <-1.0, 1.0>^2 to <0, width> x <0, height>
	glViewport(0, 0, width, height);
	// GL_LOWER_LEFT (OpenGL) or GL_UPPER_LEFT (DirectX, Windows) and GL_NEGATIVE_ONE_TO_ONE or GL_ZERO_TO_ONE
	//glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE); // ( odkud se poèítá obrázek , pseudohloubka)
	glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE); // ( odkud se poèítá obrázek , pseudohloubka)

}
void Rasterizer::loadMesh(const std::string& file_name, const std::string model) {
	SceneGraph scene;

	LoadOBJ(file_name, scene, materials_, true); // true - load adjacentTriangles

	// build continuous array for GL_TRIANGLES_ADJACENCY primitive mode
	TriangleWithAdjacency dst_triangle;

	std::vector<TriangleWithAdjacency> triangles;
	std::vector<Vertex> vert;

	for (SceneGraph::iterator iter = scene.begin(); iter != scene.end(); ++iter)
	{
		const std::string& node_name = iter->first;
		const auto& node = iter->second;

		const auto& mesh = std::static_pointer_cast<Mesh>(node);

		if (mesh)
		{
			for (Mesh::iterator iter = mesh->begin(); iter != mesh->end(); ++iter)
			{
				const auto& src_triangle = Triangle3i(**iter);
				std::shared_ptr<Material> material = iter.triangle_material();
				const int material_index = int(std::distance(std::begin(materials_), materials_.find(material->name())));

				for (int i = 0; i < 3; ++i) {
					Vertex new_vertex;
					new_vertex.position = src_triangle.position(i);
					new_vertex.normal = src_triangle.normal(i);
					new_vertex.tangent = src_triangle.tangent(i);
					new_vertex.texture_coord = Vector2(src_triangle.texture_coord(i).x, src_triangle.texture_coord(i).y);
					new_vertex.material_index = material_index;

					dst_triangle.vertices[i * 2].position = src_triangle.position(i);
					dst_triangle.vertices[i * 2].normal = src_triangle.normal(i);
					dst_triangle.vertices[i * 2].texture_coord = Vector2(src_triangle.texture_coord(i).x, src_triangle.texture_coord(i).y);
					dst_triangle.vertices[i * 2].tangent = src_triangle.tangent(i);
					dst_triangle.vertices[i * 2].material_index = material_index;

					dst_triangle.vertices[i * 2 + 1].position = src_triangle.adjacent_vertex_position(i).value_or(Vector3());
					dst_triangle.vertices[i * 2 + 1].normal = Vector3();
					dst_triangle.vertices[i * 2 + 1].texture_coord = Vector2();
					dst_triangle.vertices[i * 2 + 1].tangent = Vector3();
					dst_triangle.vertices[i * 2 + 1].material_index = -1;

					vert.push_back(new_vertex);
				}
				triangles.push_back(dst_triangle);
				/*
				for (int i = 0; i < 3; ++i)
				{
					std::optional<Vector3> av = src_triangle.adjacent_vertex_position(i);
					if (av.has_value())
					{
						av.value().Print();
					}
					else
					{
						printf("-\n");
					}
				}*/
			}
		}
	}
	if (model.compare("map") == 0) {
		this->loadedVerticesMap = vert;
		this->map_loaded = true;
	}
	else {
		this->loaded_triangles = triangles;
		this->loadedVertices = vert;
	}
}
void Rasterizer::loadMesh_triangles(const std::string& file_name)
{
	SceneGraph scene;

	LoadOBJ(file_name, scene, materials_, true);

	TriangleWithAdjacency dst_triangle;


	for (SceneGraph::iterator iter = scene.begin(); iter != scene.end(); ++iter)
	{
		const std::string& node_name = iter->first;
		const auto& node = iter->second;

		const auto& mesh = std::static_pointer_cast<Mesh>(node);

		if (mesh)
		{
			for (Mesh::iterator iter = mesh->begin(); iter != mesh->end(); ++iter)
			{
				const auto& src_triangle = Triangle3i(**iter);
				std::shared_ptr<Material> material = iter.triangle_material();
				const int material_index = int(std::distance(std::begin(materials_), materials_.find(material->name())));

				for (int i = 0; i < 3; ++i)
				{ 
					dst_triangle.vertices[i * 2].position = src_triangle.position(i);
					dst_triangle.vertices[i * 2].normal = src_triangle.normal(i);
					dst_triangle.vertices[i * 2].texture_coord = Vector2(src_triangle.texture_coord(i).x, src_triangle.texture_coord(i).y);
					dst_triangle.vertices[i * 2].tangent = src_triangle.tangent(i);
					dst_triangle.vertices[i * 2].color = Vector3(material->value(Map::kDiffuse).data[0], material->value(Map::kDiffuse).data[1], material->value(Map::kDiffuse).data[2]);
					dst_triangle.vertices[i * 2].material_index = material_index;

					dst_triangle.vertices[i * 2 + 1].position = src_triangle.adjacent_vertex_position(i).value_or(Vector3());
					dst_triangle.vertices[i * 2 + 1].normal = Vector3();
					dst_triangle.vertices[i * 2 + 1].texture_coord = Vector2();
					dst_triangle.vertices[i * 2 + 1].tangent = Vector3();
					dst_triangle.vertices[i * 2 + 1].color = Vector3();
					dst_triangle.vertices[i * 2 + 1].material_index = -1;
				}

				this->loaded_triangles.push_back(dst_triangle);
			}
		}
	}

}
void Rasterizer::initSurface() {

	const int no_vertices = loadedVertices.size();
	
	const int vertex_stride = sizeof(Vertex);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo); // generate vertex buffer object (one of OpenGL objects) and get the unique ID corresponding to that buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the newly created buffer to the GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * no_vertices, loadedVertices.data(), GL_STATIC_DRAW); // copies the previously defined vertex data into the buffer's memory
	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, tangent)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, texture_coord)));
	glVertexAttribIPointer(4, 1, GL_INT, vertex_stride, (void*)(offsetof(Vertex, material_index)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}
void Rasterizer::initSurfaceEnvMap() {
	if (loadedVerticesMap.size() > 0) {
		const int no_vertices_map = loadedVerticesMap.size();

		const int vertex_stride = sizeof(Vertex);

		glGenVertexArrays(1, &vao_env);
		glBindVertexArray(vao_env);

		glGenBuffers(1, &vbo_env); // generate vertex buffer object (one of OpenGL objects) and get the unique ID corresponding to that buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo_env); // bind the newly created buffer to the GL_ARRAY_BUFFER target
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * no_vertices_map, loadedVerticesMap.data(), GL_STATIC_DRAW); // copies the previously defined vertex data into the buffer's memory
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, position)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, normal)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, tangent)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, texture_coord)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}
	else {
		printf("Environmental map did not load properly");
	}
}
void Rasterizer::initSurfaceTriangles() {

	const int no_vertices = loaded_triangles.size() * 6;

	const int vertex_stride = sizeof(Vertex);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo); // generate vertex buffer object (one of OpenGL objects) and get the unique ID corresponding to that buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the newly created buffer to the GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * no_vertices, loaded_triangles.data(), GL_STATIC_DRAW); // copies the previously defined vertex data into the buffer's memory
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, normal)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, tangent)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, texture_coord)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(offsetof(Vertex, color)));
	glVertexAttribIPointer(5, 1, GL_INT, vertex_stride, (void*)(offsetof(Vertex, material_index)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glBindVertexArray(0);

}
void Rasterizer::initShaders() {
	// ------------------------- BASIC SHADER -----------------------------------// 
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::vector<char> shader_source;
	if (loadShader("basic_shader.vert", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(vertex_shader, 1, &tmp, nullptr);
		glCompileShader(vertex_shader);
	}
	checkShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (loadShader("basic_shader.frag", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(fragment_shader, 1, &tmp, nullptr);
		glCompileShader(fragment_shader);
	}
	checkShader(fragment_shader);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	// TODO check linking
	glUseProgram(shader_program); 

	// ------------------------- DEPTH SHADER -----------------------------------// 

	vertex_shader_shadow = glCreateShader(GL_VERTEX_SHADER);
	if (loadShader("shadow_shader.vert", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(vertex_shader_shadow, 1, &tmp, nullptr);
		glCompileShader(vertex_shader_shadow);
	}
	checkShader(vertex_shader_shadow);

	fragment_shader_shadow = glCreateShader(GL_FRAGMENT_SHADER);
	if (loadShader("shadow_shader.frag", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(fragment_shader_shadow, 1, &tmp, nullptr);
		glCompileShader(fragment_shader_shadow);
	}
	checkShader(fragment_shader_shadow);

	shadow_program_ = glCreateProgram();
	glAttachShader(shadow_program_, vertex_shader_shadow);
	glAttachShader(shadow_program_, fragment_shader_shadow);
	glLinkProgram(shadow_program_);

	// ------------------------- ENV SHADER -----------------------------------// 
	
	vertex_shader_env = glCreateShader(GL_VERTEX_SHADER);
	if (loadShader("env_shader.vert", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(vertex_shader_env, 1, &tmp, nullptr);
		glCompileShader(vertex_shader_env);
	}
	checkShader(vertex_shader_env);

	fragment_shader_env = glCreateShader(GL_FRAGMENT_SHADER);
	if (loadShader("env_shader.frag", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(fragment_shader_env, 1, &tmp, nullptr);
		glCompileShader(fragment_shader_env);
	}
	checkShader(fragment_shader_env);

	env_program = glCreateProgram();
	glAttachShader(env_program, vertex_shader_env);
	glAttachShader(env_program, fragment_shader_env);
	glLinkProgram(env_program);

	// ------------------------- STENCIL SHADER -----------------------------------// 

	vertex_shader_stencil = glCreateShader(GL_VERTEX_SHADER);
	if (loadShader("stencil_shader.vert", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(vertex_shader_stencil, 1, &tmp, nullptr);
		glCompileShader(vertex_shader_stencil);
	}
	checkShader(vertex_shader_stencil);

	geometry_shader_stencil = glCreateShader(GL_GEOMETRY_SHADER);
	if (loadShader("stencil_shader.geom", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(geometry_shader_stencil, 1, &tmp, nullptr);
		glCompileShader(geometry_shader_stencil);
	}
	checkShader(geometry_shader_stencil);

	fragment_shader_stencil = glCreateShader(GL_FRAGMENT_SHADER);
	if (loadShader("stencil_shader.frag", shader_source) == S_OK)
	{
		const char* tmp = static_cast<const char*>(&shader_source[0]);
		glShaderSource(fragment_shader_stencil, 1, &tmp, nullptr);
		glCompileShader(fragment_shader_stencil);
	}
	checkShader(fragment_shader_stencil);

	stencil_program = glCreateProgram();
	glAttachShader(stencil_program, vertex_shader_stencil);
	glAttachShader(stencil_program, geometry_shader_stencil);
	glAttachShader(stencil_program, fragment_shader_stencil);
	glLinkProgram(stencil_program);

}
/* load shader code from the text file */
int Rasterizer::loadShader(const std::string& file_name, std::vector<char>& shader)
{
	FILE* file = fopen(file_name.c_str(), "rt");

	if (!file)
	{
		printf("IO error: File '%s' not found.\n", file_name.c_str());

		return S_FALSE;
	}

	int result = S_FALSE;

	const size_t file_size = static_cast<size_t>(GetFileSize64(file_name.c_str()));

	if (file_size < 1)
	{
		printf("Shader error: File '%s' is empty.\n", file_name.c_str());
	}
	else
	{
		/* in glShaderSource we don't set the length in the last parameter,
		so the string must be null terminated, therefore +1 and reset to 0 */
		shader.clear();
		shader.resize(file_size + 1);

		size_t bytes = 0; // number of already loaded bytes

		do
		{
			bytes += fread(shader.data(), sizeof(char), file_size, file);
		} while (!feof(file) && (bytes < file_size));

		if (!feof(file) && (bytes != file_size))
		{
			printf("IO error: Unexpected end of file '%s' encountered.\n", file_name.c_str());
		}
		else
		{
			printf("Shader file '%s' loaded successfully.\n", file_name.c_str());
			result = S_OK;
		}
	}

	fclose(file);
	file = nullptr;

	return result;
}
/* check shader for completeness */
GLint Rasterizer::checkShader(const GLenum shader)
{
	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	printf("Shader compilation %s.\n", (status == GL_TRUE) ? "was successful" : "FAILED");

	if (status == GL_FALSE)
	{
		int info_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
		char* info_log = new char[info_length];
		memset(info_log, 0, sizeof(*info_log) * info_length);
		glGetShaderInfoLog(shader, info_length, &info_length, info_log);

		printf("Error log: %s\n", info_log);

		SAFE_DELETE_ARRAY(info_log);
	}

	return status;
}
void Rasterizer::initCamera(int width, int height, float FOV_y, Vector3 view_from, Vector3 view_at) {
	camera = Camera(width, height, FOV_y, view_from, view_at);
}
void Rasterizer::initLight(Vector3 position, float intensity, bool move){
	light = Light(position, intensity, move);
}

