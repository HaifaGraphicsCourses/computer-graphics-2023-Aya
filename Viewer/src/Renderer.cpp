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
	float max = 0.0f,maxx = 0.0f,maxy = 0.0f,minx = 0.0f,miny = 0.0f,max1 = 0.0f,max2 = 0.0f,avgx = 0.0f,avgy = 0.0f,deltax = 0.0f,deltay = 0.0f,deltaz = 0.0f;
	glm::mat4 transformation
	(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	DrawLine(glm::vec2(0, half_height), glm::vec2(viewport_width, half_height), glm::vec3(0.0f, 0.0f, 1.0f));
	DrawLine(glm::vec2(half_width, 0), glm::vec2(half_width, viewport_height), glm::vec3(1.0f, 0.0f, 0.0f));

	if (scene.GetModelCount() > 0)
	{
		MeshModel& model = scene.GetActiveModel(); 
		std::vector<glm::vec3> vertices = model.getVertices();
		glm::mat4 newLocal = model.LocalTransformation();
		glm::mat4 newWorld = model.WorldTransformation();
		glm::vec3 mColor = model.GetColor();
		// Check Fitting screen
		for (int i = 0; i < vertices.size(); i++)
		{
			maxx = (maxx < vertices[i].x) ? vertices[i].x : maxx;
			maxy = (maxy < vertices[i].y) ? vertices[i].y : maxy;
			miny = (miny > vertices[i].y) ? vertices[i].y : miny;
			minx = (minx > vertices[i].x) ? vertices[i].x : minx;
		}
		avgx = (maxx + minx) / 2;
		avgy = (maxy + miny) / 2;
		max1 = (maxx - minx);
		max2 = (maxy - miny);
		max = (max1 > max2) ? max1 : max2;
		deltax = float(half_width) / max;
		deltay = float(half_height) / max;
		deltaz = float( half_height) / max;
		avgx = avgx * deltax;
		avgy = avgy * deltay;
		glm::mat4 translate_mat
		(
			glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
			glm::vec4(float(half_width) - avgx, float(half_height) - avgy, 0.0f, 1.0f)
		);
		glm::mat4 translate_m_mat
		(
			glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
			glm::vec4(-avgx, -avgy, 0.0f, 1.0f)
		);
		glm::mat4 translate_center 
		(
			glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
			glm::vec4(half_width, half_height, 0.0f, 1.0f)
		);
		glm::mat4 scaling_mat 
		(
			glm::vec4(deltax, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, deltay, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, deltaz, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);

		if (model.GetIsLocal() == false)// World transform :
		{
		newWorld = model.WorldTransformation();
		transformation = newWorld * translate_mat * scaling_mat;
		model.SetWorldTransform(newWorld);
		}
		else // Local transform :
		{
			transformation = translate_center * newLocal * translate_m_mat * scaling_mat;
			model.SetLocalTransform(newLocal);
		}
		for (int faces = 0; faces < model.GetFacesCount(); faces++)
		{
			glm::vec3 p1 = vertices.at(model.GetFace(faces).GetVertexIndex(0) - 1);
			glm::vec3 p2 = vertices.at(model.GetFace(faces).GetVertexIndex(1) - 1);
			glm::vec3 p3 = vertices.at(model.GetFace(faces).GetVertexIndex(2) - 1);
			glm::vec4 p4(p1, 1.0f);
			glm::vec4 p5(p2, 1.0f);
			glm::vec4 p6(p3, 1.0f);
			p4 = transformation * p4;
			p5 = transformation * p5;
			p6 = transformation * p6;
			glm::vec2 v1(p4.x, p4.y);
			glm::vec2 v2(p5.x, p5.y);
			glm::vec2 v3(p6.x, p6.y);
			DrawLine(v1, v2, mColor);
			DrawLine(v1, v3, mColor);
			DrawLine(v2, v3, mColor);
		}
	}
	// Drawing lines from a point to many points on a circles centered around the points - Sanity Check
	/*int x0 = half_width, y0 = half_height, x1, y1, r = 500, a = 60;
	for (int i = 0; i < a; i++) {
		x1 = x0 + r * sin((2 * M_PI * i) / a);
		y1 = y0 + r * cos((2 * M_PI * i) / a);
		DrawLine(glm::ivec2(x0, y0), glm::ivec2(x1, y1), glm::vec3(1, 0, 2));
	}*/

	//Our Drawing - Heart CG (Computer Graphics)
	/*glm::ivec2 pa(200, 100);
	glm::ivec2 pb(50, 200);
	glm::ivec2 pc(50, 400);
	glm::ivec2 pd(200,350);
	glm::ivec2 pe(350, 400);
	glm::ivec2 pf(350, 200);
	glm::ivec2 pg(150, 400);
	glm::ivec2 ph(250, 400);
	glm::vec3 color1(139, 0, 0);
	DrawLine(pa, pb, color1);
	DrawLine(pb, pc, color1);
	DrawLine(pc, pg, color1);
	DrawLine(pg, pd, color1);
	DrawLine(pd, ph, color1);
	DrawLine(ph, pe, color1);
	DrawLine(pe, pf, color1);
	DrawLine(pf, pa, color1);
	glm::vec3 color2(0, 0, 0);
	glm::ivec2 p1(400, 200);
	glm::ivec2 p2(500, 200);
	glm::ivec2 p3(400, 400);
	glm::ivec2 p4(500, 400);
	DrawLine(p1, p2, color2);
	DrawLine(p1, p3, color2);
	DrawLine(p3, p4, color2);
	glm::ivec2 p5(550, 200);
	glm::ivec2 p6(650, 200);
	glm::ivec2 p7(550, 400);
	glm::ivec2 p8(650, 400);
	glm::ivec2 p9(650, 300);
	glm::ivec2 p10(620, 300);
	DrawLine(p5, p6, color2);
	DrawLine(p5, p7, color2);
	DrawLine(p7, p8, color2);
	DrawLine(p6, p9, color2);
	DrawLine(p10, p9, color2);*/

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
}
int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}
