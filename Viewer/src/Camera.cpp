#include "Camera.h"

Camera::Camera()
{
    view_transformation = glm::mat4x4(1.0f);
    projection_transformation = glm::mat4x4(1.0f);
    right = 1;
    left = -1;
    top = 1.0f;
    bottom = -1.0f;
    zNear = 0.0f;
    zFar = 1.0f;
    fovy = 45.0f;
    aspectRatio = 1.0;
    eye = glm::vec3(0.0f, 0.0f, 2.0f);
    at = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4x4 proj = glm::mat4x4(1.0f);
    glm::mat4x4 viewTransformation = glm::mat4x4(1.0f);
    glm::mat4x4 test = glm::mat4x4(1.0f);
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