#include "MeshModel.h"
#include <iostream>
using namespace std;
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{

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