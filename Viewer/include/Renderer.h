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
	int* DrawBoundingRectangleForTriangles(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool bounding_rectangles);
	void DrawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool bounding_rectangles, glm::vec3 rectangle_color, Scene scene);
	void DrawMesh(Scene scene, int j);
	void viewport(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float height);
	void DrawBoundingBox(Scene scene, MeshModel model);
	void DrawNormals(Scene scene, MeshModel model);
	void DrawFaceNormals(Scene scene, MeshModel model);
	void PaintTriangle(int rows, int cols, glm::vec3 color, bool paint_triangle, bool gray_scale, bool color_with_buffer, float zFar);
	glm::vec3 HomoToCartesian(glm::vec4 vec);
	void TransformationMultiplications(Scene scene, MeshModel model, glm::vec4 p);
	float Find_z(int _x, int  point_y, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	float CalculateArea(glm::vec3& q1, glm::vec3& q2, glm::vec3& q3);
	void Set_z(int i, int j, float z);
	float Get_z(int i, int j);
	float* color_buffer;
	float* z_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
	bool** bool_array;
	int offset_x;
	int offset_y;
	bool paintFlag;
	bool paint_triangle;
	bool gray_scale;
	bool color_with_buffer;


};
