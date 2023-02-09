#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	active_camera_index(0),
	active_model_index(0),
	active_light_index(0),
    draw_box(false),
	draw_normals(false),
	draw_face_normals(false),
	bounding_rectangles(false),
	paint_triangles(false),
	gray_scale(false),
	color_with_buffer(false)
{
	ambient_light = false;
	lighting = false;
	diffuse_light = false;
	reflection_vector = false;
	specular_light = false;
	flat_shading = false;
	phong = false;
	fog = false;
	more_than_1_light = false;
	blur = false;
	lights[0] = new Light();
	lights[1] = new Light();
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models[active_model_index];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(int index)
{
	return *cameras[index];
}

Camera& Scene::GetActiveCamera()
{
	return *cameras[active_camera_index];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index;
}
void Scene::AddLight(Light* light)
{
	//lights.push_back(light);
}
Light& Scene::GetLight(int index)
{
	return *lights[index];
}
void Scene::SetActiveLightIndex(int index)
{
	active_light_index = index;
}
int Scene::GetActiveLightIndex() const
{
	return active_light_index;
}
Light& Scene::GetActiveLight()
{
	return *lights[active_light_index];
}
