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
## Task 4:
Fragment shader that only outputs a a blue color:
```
#version 330 core

struct Material

{

	sampler2D textureMap;

};

uniform Material material;

in vec3 fragPos;

in vec3 fragNormal;

in vec2 fragTexCoords;

out vec4 frag_color;

void main()

{

	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(0, 0, 1, 1);

}
```
## Task 5
I've updated all the relevant vertex attributes and uniforms, here is the relevant code:

```
	if (scene.GetModelCount() == 0)
	    return;

	MeshModel& model = scene.GetModel(0);
	Camera& camera = scene.GetActiveCamera();
	glm::vec4 trans;
	trans = model.GetTransform() * glm::vec4(model.modelVertices[0].position,1);
	
	colorShader.use();
	colorShader.setUniform("model", model.GetTransform());
	colorShader.setUniform("view", camera.GetViewTransformation());
	colorShader.setUniform("projection", camera.GetProjectionTransformation());
	colorShader.setUniform("material.textureMap", 0);
	colorShader.setUniform("ToonShading", scene.toon_shading);
	colorShader.setUniform("levels", scene.levels);

	if (scene.lighting)
	{
		Light light = scene.GetLight(0);
		colorShader.setUniform("CameraPosition", camera.eye);
		colorShader.setUniform("AmbientLight", light.AmbientColor);
		colorShader.setUniform("DiffuseLight", light.DiffuseColor);
		colorShader.setUniform("SpecularLight",light.SpecularColor);
		colorShader.setUniform("material.ambient", model.Ka);
		colorShader.setUniform("material.diffuse", model.Kd);
		colorShader.setUniform("material.specular", model.Ks);
		colorShader.setUniform("Alpha", light.alpha);
		colorShader.setUniform("LightPosition", light.GetPosition());
	}
	texture1.bind(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(model.GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
	glBindVertexArray(0);

```

	

