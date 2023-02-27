#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>


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
	void LoadShaders();
	void LoadTextures();
	ShaderProgram lightShader;
	ShaderProgram colorShader;
	Texture2D texture1;
	Texture2D texture_normalmap;

private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void DrawCircle(const glm::ivec2& p1, double radius, const glm::vec3& color);
	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

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
