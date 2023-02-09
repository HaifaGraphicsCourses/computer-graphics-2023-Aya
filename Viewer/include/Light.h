#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>
class Light
{
private:

public:
	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;

	glm::mat4 Translation;

	glm::vec3 Ia;
	glm::vec3 Id;
	glm::vec3 Is;

	glm::vec3 I;
	glm::vec3 N;
	glm::vec3 V;
	glm::vec3 R;

	void Translate(float x, float y, float z);


	Light();
	glm::vec3 Compute_Ia(glm::vec3 Ka);

	glm::vec3 Compute_Id(glm::vec3 Kd);

	glm::vec3 Compute_Is(glm::vec3 Ks);

	glm::vec3 GetPosition();

	glm::vec3 GetLight();
};


