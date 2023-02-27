# Assignment 3 Report
***Aya Mahagna 314774639***

## Task 1:
In this assignment I replaced the software renderer implemented in the previous assignments with an OpenGL renderer.

`Renderer::InitOpenGLRendering()` Explanation:

Color buffer is loaded on the GPU as a two-dimensional texture, Two triangles covering the entire viewport are set to be drawn, Pixels are assigned z=0 and w=1, and their texture coordinates are passed to the fragment shader. based on the interpolated texture coordinates, the fragment shader finds the correct color for each pixel.

## Task2:
MeshModel was updated, and I created a struct for the layout inputs in the shader:
```
struct Vertex
{
	glm::vec3 position;

	glm::vec3 normal;

	glm::vec2 textureCoords;
};
```
