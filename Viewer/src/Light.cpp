#include "Light.h"
#include <iostream>
#include <algorithm>



Light::Light()
{

	AmbientColor = glm::vec3(0.5f, 0.5f, 0.5f);//La
	DiffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);//Ld
	SpecularColor = glm::vec3(0.5f, 0.5f, 0.5f);//Ls

	Ia = glm::vec3(0.f, 0.f, 0.f);
	Id = glm::vec3(0.f, 0.f, 0.f);
	Is = glm::vec3(0.f, 0.f, 0.f);
	Translation = glm::mat4(1.0f);

	Translation = glm::translate(Translation, { 1,4,2 });

}

void Light::Translate(float x, float y, float z)
{
	Translation = glm::translate(Translation, { x,y,z });

}

glm::vec3 Light::Compute_Ia(glm::vec3 Ka)
{
	//Ia = La * K*a
	Ia = glm::vec3(Ka.x * AmbientColor.x, Ka.y * AmbientColor.y, Ka.z * AmbientColor.z);
	return Ia;
}

glm::vec3 Light::Compute_Id(glm::vec3 Kd)
{

	Id = glm::vec3(Kd.x * DiffuseColor.x, Kd.y * DiffuseColor.y, Kd.z * DiffuseColor.z); //Id = Kd*(I*n)Ld
	//Id = glm::normalize(Id);
	float theta = glm::dot(N, I);
	Id = glm::vec3(theta * Id.x, theta * Id.y, theta * Id.z);

	if (Id.x > 1) Id.x = 1; if (Id.x < 0) Id.x = 0;
	if (Id.y > 1) Id.y = 1; if (Id.y < 0) Id.y = 0;
	if (Id.z > 1) Id.z = 1; if (Id.z < 0) Id.z = 0;
	return Id;
}

glm::vec3 Light::Compute_Is(glm::vec3 Ks)
{
	float spec = pow(std::max(dot(V, R), 0.0f), 10);

	//Is = Ls (r * v)^(alpha)*Ks
	Is = glm::vec3(SpecularColor.x * spec * Ks.x, SpecularColor.y * spec * Ks.y, SpecularColor.z * spec * Ks.z);
	return Is;

}

glm::vec3 Light::GetPosition()
{
	glm::vec3 position = { Translation[3].x,Translation[3].y ,Translation[3].z };

	return position;
}

glm::vec3 Light::GetLight()
{
	glm::vec3 color = glm::vec3(Ia.x + Id.x + Is.x, Ia.y + Id.y + Is.y, Ia.z + Id.z + Is.z);
	return color;
}