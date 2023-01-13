#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;
	void SetOrthographicProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetPerspectiveProjection( float fovy,  float aspectRatio,  float zNear,  float zFar);
	int right, left;
	int top, bottom;
	float zNear, zFar;
	float fovy;
	float aspectRatio;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	glm::mat4x4 viewTransformation;
	glm::mat4x4 test;
	glm::mat4x4 proj;
private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
};
