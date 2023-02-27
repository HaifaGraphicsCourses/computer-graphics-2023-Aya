#include <cmath>
#include <algorithm>
#include "Renderer.h"
#include "InitShader.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
	paint_triangle = false;
	gray_scale = false;
	color_with_buffer = false;
	paintFlag = false;
	//array for 2d bolean
	bool_array = new bool* [(viewport_width + 1)];
	for (int i = 0; i < viewport_width + 1; i++)
		bool_array[i] = new bool[(viewport_height + 1)];
}

Renderer::~Renderer()
{
	delete[] color_buffer;
	delete[] z_buffer;
	for (int i = 0; i < viewport_width + 1; i++)  
		delete[] bool_array[i];
	delete[] bool_array;          
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;
	
	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y, dx, dy, sx, sy, error, e2;
	dx = abs(x2 - x1); 
	sx = x1 < x2 ? 1 : -1;
	dy = (-1)*abs(y2 - y1);
	sy = y1 < y2 ? 1 : -1;
	error = dx + dy;
	while (true)
	{
		PutPixel(x1, y1, color);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = 2 * error;
		if (e2 >= dy)
		{
			if (x1 == x2) 
				break;
			error += dy;
			x1 += sx;
		}
		if (e2 <= dx)
		{
			if (y1 == y2)
				break;
			error += dx;
			y1 += sy;
		}
	}
}
// Drawing a circle from given point and radius and color 
void Renderer::DrawCircle(const glm::ivec2& p1, double r, const glm::vec3& color)
{
	for (int i = p1.x - r; i <= p1.x + r; i++) {
		PutPixel(i, sqrt(r * r - (i - p1.x) * (i - p1.x)) + p1.y, color);
		PutPixel(i, (sqrt(r * r - (i - p1.x) * (i - p1.x)) + p1.y) - 2 * ((sqrt(r * r - (i - p1.x) * (i - p1.x)) + p1.y) - p1.y), color);
	}
	for (int i = p1.y - r; i <= p1.y + r; i++) {
		PutPixel(sqrt(r * r - (i - p1.y) * (i - p1.y)) + p1.x, i, color);
		PutPixel((sqrt(r * r - (i - p1.y) * (i - p1.y)) + p1.x) - 2 * ((sqrt(r * r - (i - p1.y) * (i - p1.y)) + p1.x) - p1.x), i, color);
	}
}
void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[(3 * w * h)];
	z_buffer = new float[w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
			z_buffer[Z_INDEX(viewport_width, i, j)] = INFINITY;

		}
	}
}

void Renderer::Render(Scene& scene)
{
	if (scene.GetModelCount() == 0)
		return;

	MeshModel& model = scene.GetModel(0);
	Camera& camera = scene.GetActiveCamera();
	glm::vec4 temp;
	temp = model.GetTransform() * glm::vec4(model.modelVertices[0].position, 1);
	cout << "model position: " << temp.x << " " << temp.y << " " << temp.z << " " << endl;
	colorShader.use();
	colorShader.setUniform("model", model.GetTransform());
	colorShader.setUniform("view", camera.GetViewTransformation());
	colorShader.setUniform("projection", camera.GetProjectionTransformation());
	colorShader.setUniform("material.textureMap", 0);
	colorShader.setUniform("ToonShading", scene.toon_shading);
	colorShader.setUniform("levels", scene.levels);
	colorShader.setUniform("UseTexture", scene.use_texture);

	if (scene.lighting)
	{
		Light light = scene.GetLight(0);

		colorShader.setUniform("AmbientLight", light.AmbientColor);
		colorShader.setUniform("DiffuseLight", light.DiffuseColor);
		colorShader.setUniform("SpecularLight", light.SpecularColor);
		colorShader.setUniform("material.ambient", model.Ka);
		colorShader.setUniform("material.diffuse", model.Kd);
		colorShader.setUniform("material.specular", model.Ks);
		colorShader.setUniform("Alpha", light.alpha);
		colorShader.setUniform("LightPosition", light.GetPosition());
		colorShader.setUniform("CameraPosition", camera.eye);
	}
	texture1.bind(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(model.GetVao());
	glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
	glBindVertexArray(0);
	texture1.unbind(0);
	colorShader.setUniform("color", glm::vec3(0, 0, 0));
}
	void Renderer::SetSize(int width, int height)
	{
		viewport_width = width;
		viewport_height = height;
	}


	int Renderer::GetViewportWidth() const
	{
		return viewport_width;
	}

	int Renderer::GetViewportHeight() const
	{
		return viewport_height;
	}
	void Renderer::LoadShaders()
	{
		colorShader.loadShaders("vshader.glsl", "fshader.glsl");
	}

	void Renderer::LoadTextures()
	{
		if (!texture1.loadTexture("C:\\Users\\aya19\\Desktop\\University\\ComputerScience\\Computer Graphics\\computer-graphics-2023-aya-rima\Data\\crate.jpg", true))
		{
			texture1.loadTexture("C:\\Users\\aya19\\Desktop\\University\\ComputerScience\\Computer Graphics\\computer-graphics-2023-aya-rima\Data\\crate.jpg", true);
		}
	}
