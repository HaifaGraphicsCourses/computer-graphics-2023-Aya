#include "MeshModel.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	isLocal(true)
{
	localTransform = worldTransform = localTranslate = worldTranslate =localScale = worldScale = localRotate  = worldRotate = {
	   glm::vec4(1.0f,0.0f,0.0f,0.0f),
	   glm::vec4(0.0f,1.0f,0.0f,0.0f),
	   glm::vec4(0.0f,0.0f,1.0f,0.0f),
	   glm::vec4(0.0f,0.0f,0.0f,1.0f)
	};
	modelColor = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int j = 0; j < vertices.size(); j++) {
		std::cout << "vertices " << j << "(X: " << vertices[j][0] << " ,Y: " << vertices[j][1] << " ,Z: " << vertices[j][2]<<")" << std::endl;
	}
	for (int i = 0; i < faces.size(); i++) {
		std::cout << "faces "  << ": (" << faces[i].GetVertexIndex(0) << ", " << faces[i].GetVertexIndex(1) << ", " << faces[i].GetVertexIndex(2)<<")" << std::endl;
	}
}

MeshModel::~MeshModel()
{
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
	glm::mat4x4 worldTrans = worldTranslate * worldRotate* worldScale;
	return worldTrans;
}
