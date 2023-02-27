# Assignment 3 Report
***Aya Mahagna 314774639***

## Task 1:
In this assignment I replaced the software renderer implemented in the previous assignments with an OpenGL renderer.

`Renderer::InitOpenGLRendering()` Explanation:

Color buffer is loaded on the GPU as a two-dimensional texture, Two triangles covering the entire viewport are set to be drawn, Pixels are assigned z=0 and w=1, and their texture coordinates are passed to the fragment shader. based on the interpolated texture coordinates, the fragment shader finds the correct color for each pixel.

## Task 2:
MeshModel was updated, and I created a struct for the layout inputs in the shader:
```
struct Vertex
{
	glm::vec3 position;

	glm::vec3 normal;

	glm::vec2 textureCoords;
};
```
## Task 3:
I wrote a vertex shader that applies all the transformations.  

```
#version 330 core

layout(location = 0) in vec3 pos;

layout(location = 1) in vec3 normal;

layout(location = 2) in vec2 texCoords;


uniform mat4 model;

uniform mat4 view;

uniform mat4 projection;


out vec3 orig_fragPos;

out vec3 fragPos;

out vec3 fragNormal;

out vec2 fragTexCoords;



void main()

{

	orig_fragPos = vec3(vec4(pos, 1.0f));

	fragPos = vec3(model * vec4(pos, 1.0f));

	fragNormal = mat3(model) * normal;

	fragTexCoords = texCoords;

	gl_Position = projection * view *  model * vec4(pos, 1.0f);

}
```
