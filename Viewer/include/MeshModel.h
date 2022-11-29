#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	std::vector<glm::vec3>& getVertices();

	void WorldTranslate(float x, float y, float z);

	void WorldScale(float x, float y, float z);

	void WorldRotate(float angle, glm::vec3 axis);

	void LocalTranslate(float x, float y, float z);

	void LocalScale(float x, float y, float z);

	void localRotatation(float angle, glm::vec3 axis);

	const glm::mat4x4& LocalTransformation() const;

	const glm::mat4x4& WorldTransformation() const;

	glm::mat4x4 GetTransform()
	{
		return   localTransform * worldTransform;
	}
	glm::vec3 GetColor()
	{
		return modelColor;
	}
	void SetLocalTransform(glm::mat4x4 newTransform)
	{
		localTransform = newTransform;
	}

	void SetWorldTransform(glm::mat4x4 newTransform)
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

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
	glm::vec3 modelColor;
	glm::mat4x4 localTransform;
	glm::mat4x4 worldTransform;
	glm::mat4x4 localTranslate;
	glm::mat4x4 worldTranslate;
	glm::mat4x4 localScale;
	glm::mat4x4 worldScale;
	glm::mat4x4 localRotate;
	glm::mat4x4 worldRotate;
	bool isLocal;
};
