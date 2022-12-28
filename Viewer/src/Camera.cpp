#include "Camera.h"

Camera::Camera()
{
    view_transformation = glm::mat4x4(1.0f);
    projection_transformation = glm::mat4x4(1.0f);
}

Camera::~Camera()
{
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
    view_transformation = glm::lookAt(eye, at, up);
}

void Camera::SetOrthographicProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
    projection_transformation = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void Camera::SetPerspectiveProjection(float fovy, float aspectRatio,float zNear,float zFar)
{
    projection_transformation = glm::perspective(fovy, aspectRatio, zNear, zFar);
}