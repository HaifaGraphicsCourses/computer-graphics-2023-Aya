#include "MeshModel.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	textureCoords(textureCoords),
	model_name(model_name)
{
	localTransform = worldTransform = localTranslate = worldTranslate =localScale = worldScale = localRotate  = worldRotate = glm::mat4(1.0f);
	modelColor = glm::vec3(0.0f, 0.0f, 0.0f);
	worldAxes = false;
	localAxes = false;
	Ka = glm::vec3(0.0f, 0, 0);
	Kd = glm::vec3(1, 0, 0);
	Ks = glm::vec3(1.0f, 1, 1);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec3(dist(mt), dist(mt), dist(mt));
	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;
			Vertex vertex;
			vertex.position = vertices[vertexIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}
			if (normals.size() > 0) {
				vertex.normal = normals[normalIndex];
			}
			modelVertices.push_back(vertex);
		}
	}
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	/*for (int j = 0; j < vertices.size(); j++) {
		std::cout << "vertices " << j << "(X: " << vertices[j][0] << " ,Y: " << vertices[j][1] << " ,Z: " << vertices[j][2]<<")" << std::endl;
	}
	for (int i = 0; i < faces.size(); i++) {
		std::cout << "faces "  << ": (" << faces[i].GetVertexIndex(0) << ", " << faces[i].GetVertexIndex(1) << ", " << faces[i].GetVertexIndex(2)<<")" << std::endl;
	}*/
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

std::vector<glm::vec3>& MeshModel::getVertices() {
	return vertices;
}
float MeshModel::GetVertex(int index, int coordinate)
{
	return vertices[index][coordinate];
}
float MeshModel::GetNormal(int index, int coordinate)
{
	return normals[index][coordinate];
}

std::vector<glm::vec3> MeshModel::GetNormals()
{
	return normals;
}

void MeshModel::WorldTranslate(float x, float y, float z)
{
	worldTranslate = glm::translate(worldTranslate, { x,y,z });
	worldTransform = worldTranslate * worldRotate * worldScale;

}
void MeshModel::WorldScale(float x, float y, float z)
{
	worldScale = glm::scale(worldScale, { x,y,z });
	worldTransform = worldTranslate * worldRotate * worldScale;
}
void MeshModel::WorldRotate(float angle, glm::vec3 axis)
{
	worldRotate = glm::rotate(worldRotate, glm::radians(angle), axis);
	worldTransform = worldTranslate * worldRotate * worldScale;
}
void MeshModel::LocalTranslate(float x, float y, float z)
{
	localTranslate = glm::translate(localTranslate, { x,y,z });
	localTransform = localTranslate * localRotate * localScale;
}
void MeshModel::LocalScale(float x, float y, float z)
{
	localScale = glm::scale(localScale, { x,y,z });
	localTransform = localTranslate * localRotate * localScale;
}
void MeshModel::localRotatation(float angle, glm::vec3 axis)
{
	localRotate = glm::rotate(localRotate, glm::radians(angle), axis);
	localTransform = localTranslate * localRotate * localScale;
}
const glm::mat4x4& MeshModel::LocalTransformation() const
{
	glm::mat4x4 localtrans = localTranslate * localRotate * localScale;
	return localtrans;
}

const glm::mat4x4& MeshModel::WorldTransformation() const
{
	return worldTranslate * worldRotate* worldScale;
	
}
void MeshModel::ResetTransformations()
{
	localTransform = glm::mat4(1.0f);
	worldTransform = glm::mat4(1.0f);
	localTranslate = glm::mat4(1.0f);
	localRotate = glm::mat4(1.0f);
	localScale = glm::mat4(1.0f);
	worldTranslate = glm::mat4(1.0f);
	worldRotate = glm::mat4(1.0f);
	worldScale = glm::mat4(1.0f);
}
glm::vec3 MeshModel::GetPosition()
{
	glm::mat4 hi = GetTransform();
	glm::vec3 position = { GetTransform()[3].x,GetTransform()[3].y ,GetTransform()[3].z };
	return position;
}
GLuint MeshModel::GetVao() const
{
	return vao;
}
const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}
void MeshModel::SetPlane()
{
	for (int i = 0; i < modelVertices.size(); i++)
	{
		modelVertices.at(i).textureCoords.x = modelVertices.at(i).position.x;
		modelVertices.at(i).textureCoords.y = modelVertices.at(i).position.y;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_VERTEX_ARRAY, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelVertices.size() * sizeof(Vertex), &modelVertices[0]);
	glBindVertexArray(0);
}