#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void SetSize(int width, int height);
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void DrawCircle(const glm::ivec2& p1, double radius, const glm::vec3& color);
	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();
	void WorldCoordinates(Scene scene, int j);
	void LocalCoordinates(Scene scene, int j);
	void DrawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
	void DrawMeshModel(Scene scene, int j);
	void viewport(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float height);
	void DrawBoundingBox(Scene scene, MeshModel model);
	void DrawNormals(Scene scene, MeshModel model);
	void DrawFaceNormals(Scene scene, MeshModel model);
	glm::vec3 HomoToCartesian(glm::vec4 vec);
	float* color_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
};
