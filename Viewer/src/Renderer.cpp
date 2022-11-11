#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
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
	color_buffer = new float[3 * w * h];
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
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	// Drawing lines from a point to many points on a circles centered around the points - Sanity Check
	int x0 = half_width, y0 = half_height, x1, y1, r = 500, a = 60;
	for (int i = 0; i < a; i++) {
		x1 = x0 + r * sin((2 * M_PI * i) / a);
		y1 = y0 + r * cos((2 * M_PI * i) / a);
		DrawLine(glm::ivec2(x0, y0), glm::ivec2(x1, y1), glm::vec3(1, 0, 2));
	}

	//Drawing Lines
	/*glm::ivec2 p1(150, 150);
	glm::ivec2 p2(200, 200);
	glm::ivec2 p3(500, 150);
	glm::ivec2 p4(150, 500);
	glm::ivec2 p6(250, 450);
	glm::ivec2 p7(450, 250);
	glm::ivec2 p8(350, 100);
	glm::vec3 color1(0, 120, 0);
	glm::vec3 color2(2, 0, 0);
	glm::vec3 color3(2, 0.8, 0.243);
	glm::vec3 color4(0.21, 0.78, 0.12);
	glm::vec3 color5(0.9, 0.52, 10);
	glm::vec3 color6(0.8, 0.1, 0.578);
	DrawLine(p1, p2, color1);
	DrawLine(p1, p3, color2);
	DrawLine(p1, p4, color3);
	DrawLine(p1, p6, color4);
	DrawLine(p1, p7, color5);
	DrawLine(p1, p8, color6);*/

	// Drawing the 2 circles
	/*
	glm::ivec2 p1(100, 170);
	glm::ivec2 p2(160, 190);
	glm::vec3 color1(0, 1, 200);
	glm::vec3 color2(1, 0, 3);
	DrawCircle(p1, 110, color1);
	DrawCircle(p2, 80, color2);*/

	//Our Drawing
	/*glm::ivec2 p1(0, 0);
	glm::ivec2 p2(0,0);
	glm::vec3 color(0, 1, 240);
	for (int i = 0; i < 10; i++) {
		p1.x = 12 * 20;
		p1.y = 800 - (100 + i);
		p2.x = 34 * 20 - 1;
		p2.y = 800 - (100 + i);
		DrawLine(p1, p2, color);
		p1.x = 6 * 20;
		p1.y = 800 - (120 + i);
		p2.x = 41 * 20 - 1;
		p2.y = 800 - (120 + i);
		DrawLine(p1, p2, color);
		p1.x = 3 * 20;
		p1.y = 800 - (140 + i);
		p2.x = 6 * 20 - 1;
		p2.y = 800 - (140 + i);
		DrawLine(p1, p2, color);
		p1.x = 19 * 20;
		p2.x = 25 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 35 * 20;
		p2.x = 50 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 2 * 20;
		p1.y = 800 - (160 + i);
		p2.x = 5 * 20 - 1;
		p2.y = 800 - (160 + i);
		DrawLine(p1, p2, color);
		p1.x = 6 * 20;
		p2.x = 18 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 19 * 20;
		p2.x = 22 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 25 * 20;
		p2.x = 48 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (180 + i);
		p2.y = 800 - (180 + i);
		p1.x = 2 * 20;
		p2.x = 9 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 15 * 20;
		p2.x = 21 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 22 * 20;
		p2.x = 44 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (200 + i);
		p2.y = 800 - (200 + i);
		p1.x = 4 * 20;
		p2.x = 8 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 9 * 20;
		p2.x = 10 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 13 * 20;
		p2.x = 14 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 16 * 20;
		p2.x = 25 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 33 * 20;
		p2.x = 42 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (220 + i);
		p2.y = 800 - (220 + i);
		p1.x = 7 * 20;
		p2.x = 8 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 9 * 20;
		p2.x = 15 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 19 * 20;
		p2.x = 22 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 25 * 20;
		p2.x = 28 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 25 * 20;
		p2.x = 28 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 30 * 20;
		p2.x = 33 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 37 * 20;
		p2.x = 44 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (240 + i);
		p2.y = 800 - (240 + i);
		p1.x = 15 * 20;
		p2.x = 25 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 27 * 20;
		p2.x = 30 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 35 * 20;
		p2.x = 48 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (260 + i);
		p2.y = 800 - (260 + i);
		p1.x = 14 * 20;
		p2.x = 22 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 24 * 20;
		p2.x = 27 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 32 * 20;
		p2.x = 35 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 36 * 20;
		p2.x = 54 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (280 + i);
		p2.y = 800 - (280 + i);
		p1.x = 2 * 20;
		p2.x = 14 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 22 * 20;
		p2.x = 24 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 25 * 20;
		p2.x = 27 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 29 * 20;
		p2.x = 35 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 36 * 20;
		p2.x = 58 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (300 + i);
		p2.y = 800 - (300 + i);
		p1.x = 2 * 20;
		p2.x = 3 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 8 * 20;
		p2.x = 10 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 14 * 20;
		p2.x = 15 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 16 * 20;
		p2.x = 21 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 24 * 20;
		p2.x = 25 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 27 * 20;
		p2.x = 30 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 33 * 20;
		p2.x = 35 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 36 * 20;
		p2.x = 59 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (320 + i);
		p2.y = 800 - (320 + i);
		p1.x = 9 * 20;
		p2.x = 15 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 18 * 20;
		p2.x = 24 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 25 * 20;
		p2.x = 29 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 30 * 20;
		p2.x = 32 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 33 * 20;
		p2.x = 36 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 37 * 20;
		p2.x = 55 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (340 + i);
		p2.y = 800 - (340 + i);
		p1.x = 21 * 20;
		p2.x = 23 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 24 * 20;
		p2.x = 32 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 33 * 20;
		p2.x = 36 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 37 * 20;
		p2.x = 53 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (360 + i);
		p2.y = 800 - (360 + i);
		p1.x = 4 * 20;
		p2.x = 21 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 23 * 20;
		p2.x = 30 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 32 * 20;
		p2.x = 36 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 37 * 20;
		p2.x = 52 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (380 + i);
		p2.y = 800 - (380 + i);
		p1.x = 6 * 20;
		p2.x = 8 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 18 * 20;
		p2.x = 29 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 30 * 20;
		p2.x = 37 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 38 * 20;
		p2.x = 54 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (400 + i);
		p2.y = 800 - (400 + i);
		p1.x = 8 * 20;
		p2.x = 15 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 20 * 20;
		p2.x = 28 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 29 * 20;
		p2.x = 37 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 38 * 20;
		p2.x = 55 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (420 + i);
		p2.y = 800 - (420 + i);
		p1.x = 15 * 20;
		p2.x = 18 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 24 * 20;
		p2.x = 26 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 28 * 20;
		p2.x = 37 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 38 * 20;
		p2.x = 57 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (440 + i);
		p2.y = 800 - (440 + i);
		p1.x = 18 * 20;
		p2.x = 24 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 27 * 20;
		p2.x = 36 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 27 * 20;
		p2.x = 36 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 37 * 20;
		p2.x = 58 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (460 + i);
		p2.y = 800 - (460 + i);
		p1.x = 22 * 20;
		p2.x = 23 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 26 * 20;
		p2.x = 35 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 36 * 20;
		p2.x = 52 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 54 * 20;
		p2.x = 59 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (480 + i);
		p2.y = 800 - (480 + i);
		p1.x = 21 * 20;
		p2.x = 22 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 25 * 20;
		p2.x = 34 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 35 * 20;
		p2.x = 50 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (500 + i);
		p2.y = 800 - (500 + i);
		p1.x = 20 * 20;
		p2.x = 21 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 24 * 20;
		p2.x = 28 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 34 * 20;
		p2.x = 49 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (520 + i);
		p2.y = 800 - (520 + i);
		p1.x = 19 * 20;
		p2.x = 20 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 23 * 20;
		p2.x = 26 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 42 * 20;
		p2.x = 52 * 20 - 1;
		DrawLine(p1, p2, color);		
		p1.y = 800 - (540 + i);
		p2.y = 800 - (540 + i);
		p1.x = 19 * 20;
		p2.x = 20 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 22 * 20;
		p2.x = 25 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.y = 800 - (560 + i);
		p2.y = 800 - (560 + i);
		p1.x = 18 * 20;
		p2.x = 19 * 20 - 1;
		DrawLine(p1, p2, color);
		p1.x = 22 * 20;
		p2.x = 23 * 20 - 1;
		DrawLine(p1, p2, color);
	}
	*/
}
int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}