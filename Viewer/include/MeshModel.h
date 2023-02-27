#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include <glad/glad.h>
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	std::vector<glm::vec3>& getVertices();
	float GetVertex(int index, int coordinate);
	void WorldTranslate(float x, float y, float z);
	void WorldScale(float x, float y, float z);
	void WorldRotate(float angle, glm::vec3 axis);
	void LocalTranslate(float x, float y, float z);
	void LocalScale(float x, float y, float z);
	void localRotatation(float angle, glm::vec3 axis);
	const glm::mat4x4& LocalTransformation() const;
	const glm::mat4x4& WorldTransformation() const;
	void ResetTransformations();
	float GetNormal(int index, int coordinate);
	std::vector<glm::vec3> GetNormals();
	int getVerticesSize()  {
		return vertices.size();
	}
	glm::mat4x4 GetTransform()
	{
		return   localTransform * worldTransform;
	}
	glm::vec3 GetColor()
	{
		return modelColor;
	}
	void SetColor(const glm::vec3 new_color)
	{
		modelColor=new_color;
	}
	void SetLocalScale(const glm::mat4x4 newTransform)
	{
		localScale = newTransform;
	}
	void SetLocalTranslate(const glm::mat4x4 newTransform)
	{
		localTranslate = newTransform;
	}
	void SetLocalRotate(const glm::mat4x4 newTransform)
	{
		localRotate = newTransform;
	}
	void SetWorldScale(const glm::mat4x4 newTransform)
	{
		worldScale = newTransform;
	}
	void SetWorldTranslate(const glm::mat4x4 newTransform)
	{
		worldTranslate = newTransform;
	}
	void SetWorldRotate(const glm::mat4x4 newTransform)
	{
		worldRotate = newTransform;
	}
	void SetLocalTransform(const glm::mat4x4 newTransform)
	{
		localTransform = newTransform;
	}

	void SetWorldTransform(const glm::mat4x4 newTransform)
	{
		worldTransform=newTransform;
	}
	bool GetIsLocal()
	{
		return isLocal;
	}
	void SetIsLocal(bool newValue)
	{
		isLocal = newValue;
	}
	glm::vec3 MeshModel::GetPosition();
	GLuint GetVao() const;
	const std::vector<Vertex>& GetModelVertices();
	void MeshModel::SetPlane();

	bool worldAxes;
	bool localAxes;
	glm::mat4x4 localTransform;
	glm::mat4x4 worldTransform;
	glm::mat4x4 localTranslate;
	glm::mat4x4 worldTranslate;
	glm::mat4x4 localScale;
	glm::mat4x4 worldScale;
	glm::mat4x4 localRotate;
	glm::mat4x4 worldRotate;
	bool isLocal;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 color;
	GLuint vbo;
	GLuint vao;
	std::vector<Vertex> modelVertices;

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
	glm::vec3 modelColor;
	std::vector<glm::vec2> textureCoords;

	
};
