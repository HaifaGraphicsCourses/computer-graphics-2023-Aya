#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "Light.h"
#include "MeshModel.h"
using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera();
	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;
	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;
	void Scene::AddLight(Light* light);
	Light& Scene::GetLight(int index);
	void SetActiveLightIndex(int index);
	int GetActiveLightIndex() const;
	Light& GetActiveLight();

	bool draw_box;
	bool draw_normals;
	bool draw_face_normals;
	bool bounding_rectangles;
	bool paint_triangles;
	bool gray_scale;
	bool color_with_buffer;
	bool lighting;
	bool ambient_light;
	bool diffuse_light;
	bool reflection_vector;
	bool specular_light;
	bool flat_shading;
	bool phong;
	bool fog;
	bool more_than_1_light;
	bool blur;
	bool normal_map;
	bool toon_shading;
	float levels;
	bool use_texture;


private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	Light* lights[2];
	int active_camera_index;
	int active_model_index;
	int active_light_index;
}; 