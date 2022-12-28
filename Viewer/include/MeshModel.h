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
	int getVerticesSize() const {
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
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
	glm::vec3 modelColor;
	
};
