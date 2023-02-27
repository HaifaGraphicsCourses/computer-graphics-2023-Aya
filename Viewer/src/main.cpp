#include <cmath>
#include <imgui/imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>


bool local_axes, world_axes, is_ortho, is_boundingBox, is_normals = false;
int Orthographic = 0;
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int windowWidth = 1900, windowHeight = 1100;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);

	int frameBufferWidth, frameBufferHeight;
	int width = 1920, height = 1080;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	renderer.LoadShaders();
	renderer.LoadTextures();
	Camera camera;
	scene.AddCamera(std::make_shared<Camera>(camera));
	scene.SetActiveCameraIndex(0);

	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	// very importent!! initialization of glad
	// https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	/*	if (scene.GetCameraCount() != 0) {
			renderer.SetSize(frameBufferWidth, frameBufferHeight);
		}*/
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown['A'])
		{
			scene.GetModel(0).LocalTranslate(-0.05, 0, 0);
		}
		else if (io.KeysDown['D'])
		{
			scene.GetModel(0).LocalTranslate(0.05, 0, 0);
		}
		else if (io.KeysDown['W'])
		{
			scene.GetModel(0).LocalTranslate(0, 0.05, 0);
		}
		else if (io.KeysDown['S'])
		{
			scene.GetModel(0).LocalTranslate(0, -0.05, 0);
		}

		else if (io.KeysDown['J'])
		{
			scene.GetModel(0).localRotatation(5, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (io.KeysDown['L'])
		{
			scene.GetModel(0).localRotatation(-5, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (io.KeysDown['I'])
		{
			scene.GetModel(0).localRotatation(5, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (io.KeysDown['K'])
		{
			scene.GetModel(0).localRotatation(-5, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		else if (io.KeysDown['T'])
		{
			scene.GetModel(0).LocalScale(1.1, 1.1, 1.1);
		}
		else if (io.KeysDown['Y'])
		{
			scene.GetModel(0).LocalScale(0.9, 0.9, 0.9);
		}

	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	ImGui::End();
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// Transformation window
	{
		ImGui::SetNextWindowSize(ImVec2(400, 430));
		ImGui::Begin("Transformations Control:");

		ImGui::Checkbox("Demo Window", &show_demo_window);
		ImGui::Checkbox("Another Window", &show_another_window);
		static glm::vec3 world_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 world_rotation(0.0f, 0.0f, 0.0f);
		static float world_scale = 1.0f;
		static glm::vec3 local_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 local_rotation(0.0f, 0.0f, 0.0f);
		static float local_scale = 1.0f;
		static glm::vec3 local_prev_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 local_prev_rotation(0.0f, 0.0f, 0.0f);
		static float local_prev_scale = 1.0f;
		static glm::vec3 world_prev_translation(0.0f, 0.0f, 0.0f);
		static glm::vec3 world_prev_rotation(0.0f, 0.0f, 0.0f);
		static float world_prev_scale = 1.0f;
		ImGui::Text("        ");
		ImGui::Text("Local Transformations  ");
		ImGui::SliderFloat("Local Translate-x", &local_translation[0], -1.0f, 1.0f);
		ImGui::SliderFloat("LocalTranslate-y", &local_translation[1], -1.0f, 1.0f);
		ImGui::SliderFloat("Local Translate-z", &local_translation[2], -1.0f, 1.0f);
		ImGui::SliderFloat("Local Rotation-x", &local_rotation[0], -1.0f, 1.0f);
		ImGui::SliderFloat("Local Rotation-y", &local_rotation[1], -1.0f, 1.0f);
		ImGui::SliderFloat("Local Rotation-z", &local_rotation[2], -1.0f, 1.0f);
		ImGui::SliderFloat("Local Scale", &local_scale, 0.01f, 1.0f);
		ImGui::Text("World Transformations ");
		ImGui::SliderFloat("World Translate-x", &world_translation[0], -1.0f, 1.0f);
		ImGui::SliderFloat("World Translate-y", &world_translation[1], -1.0f, 1.0f);
		ImGui::SliderFloat("World Translate-z", &world_translation[2], -1.0f, 1.0f);
		ImGui::SliderFloat("World Rotation-x", &world_rotation[0], -1.0f, 1.0f);
		ImGui::SliderFloat("World Rotation-y", &world_rotation[1], -1.0f, 1.0f);
		ImGui::SliderFloat("World Rotation-z", &world_rotation[2], -1.0f, 1.0f);
		ImGui::SliderFloat("World Scale", &world_scale, 0.01f, 1.0f);
		if (local_prev_scale != local_scale || local_prev_translation != local_translation || local_prev_rotation != local_rotation) {
			scene.GetModel(0).LocalTranslate(local_translation.x - local_prev_translation.x, local_translation.y - local_prev_translation.y, local_translation.z - local_prev_translation.z);
			local_prev_scale = local_scale;
			scene.GetModel(0).localScale[0][0] = scene.GetModel(0).localScale[1][1] = scene.GetModel(0).localScale[2][2] = local_scale;
		}
		if (local_rotation.x != local_prev_rotation.x) {
			scene.GetModel(0).localRotatation(local_rotation.x - local_prev_rotation.x, { 1.0f,0.0f,0.0f });
		}
		if (local_rotation.y != local_prev_rotation.y) {
			scene.GetModel(0).localRotatation(local_rotation.y - local_prev_rotation.y, { 0.0f,1.0f,0.0f });
		}
		if (local_rotation.z != local_prev_rotation.z) {
			scene.GetModel(0).localRotatation(local_rotation.z - local_prev_rotation.z, { 0.0f,0.0f,1.0f });
		}
		local_prev_scale = local_scale;
		local_prev_translation = local_translation;
		local_prev_rotation = local_rotation;
		if ((world_prev_scale != world_scale) || (world_prev_translation != world_translation) || (world_prev_rotation != world_rotation)) {
			scene.GetModel(0).WorldTranslate(5 * world_translation.x - 5 * world_prev_translation.x, 5 * world_translation.y - 5 * world_prev_translation.y, 5 * world_translation.z - 5 * world_prev_translation.z);
			world_prev_scale = world_scale;
			scene.GetModel(0).worldScale[0][0] = scene.GetModel(0).worldScale[1][1] = scene.GetModel(0).worldScale[2][2] = world_scale;
		}
		if (world_rotation.x != world_prev_rotation.x) {
			scene.GetModel(0).WorldRotate(world_rotation.x - world_prev_rotation.x, { 1.0f,0.0f,0.0f });
		}
		if (world_rotation.y != world_prev_rotation.y) {
			scene.GetModel(0).WorldRotate(world_rotation.y - world_prev_rotation.y, { 0.0f,1.0f,0.0f });
		}
		if (world_rotation.z != world_prev_rotation.z) {
			scene.GetModel(0).WorldRotate(world_rotation.z - world_prev_rotation.z, { 0.0f,0.0f,1.0f });
		}
		world_prev_scale = world_scale;
		world_prev_translation = world_translation;
		world_prev_rotation = world_rotation;
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Reset Transformation"))
		{
			local_prev_translation = local_translation = glm::vec3{ 0,0,0 };
			local_prev_rotation = local_rotation = glm::vec3{ 0,0,0 };
			local_prev_scale = local_scale = 1.f;
			world_prev_translation = world_translation = glm::vec3{ 0,0,0 };
			world_prev_rotation = world_rotation = glm::vec3{ 0,0,0 };
			world_prev_scale = world_scale = 1.f;
			scene.GetModel(0).ResetTransformations();
		}
		ImGui::SameLine();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	Camera& camera = scene.GetActiveCamera();
	ImGui::SetNextWindowSize(ImVec2(350, 550));
	ImGui::Begin("Camera Control:");
	static int projection = 1;
	ImGui::RadioButton("Orthographic", &projection, 1); ImGui::SameLine();
	ImGui::RadioButton("Perspective", &projection, 2);
	/*ImGui::SliderInt("Right", &camera.right, -5.0f, 5.0f);
	ImGui::SliderInt("Left", &camera.left, -5.0f, 5.0f);*/
	ImGui::SliderInt("Down", &camera.bottom, -5, 5);
	ImGui::SliderInt("Up", &camera.top, -5, 5);
	ImGui::InputFloat("Left", &camera.left);
	ImGui::InputFloat("Right", &camera.right);
	ImGui::SliderFloat("Near", &camera.zNear, -1.00f, 100.0f);
	ImGui::SliderFloat("Far", &camera.zFar, -100.0f, 100.0f);

	if (projection == 1 && scene.GetModelCount())
	{
		scene.GetActiveCamera().SetOrthographicProjection(camera.left, camera.right, camera.bottom, camera.top, camera.zNear, camera.zFar);
	}
	if (projection == 2 && scene.GetModelCount())
	{
		ImGui::InputFloat("Fovy", &camera.fovy, 0.01f, 1.0f, "%.2f");
		float aspectRatio = (camera.right - camera.left) / (camera.top - camera.bottom);
		scene.GetActiveCamera().SetPerspectiveProjection(glm::radians(camera.fovy), aspectRatio,camera.zNear, camera.zFar);
	}
	//ImGui::Text("           ");
	ImGui::Text("Camera LookAt:");

	ImGui::SliderFloat("Eye-x", &camera.eye.x, -10, 10);
	ImGui::SliderFloat("Eye-y", &camera.eye.y, -10, 10);
	ImGui::SliderFloat("Eye-z", &camera.eye.z, -10, 10);
	ImGui::SliderFloat("At-x", &camera.at.x, -10, 10);
	ImGui::SliderFloat("At-y", &camera.at.y, -10, 10);
	ImGui::SliderFloat("At-z", &camera.at.z, -10, 10);
	ImGui::InputFloat3("Up", &camera.up.x);
	scene.GetActiveCamera().SetCameraLookAt(camera.eye, camera.at, camera.up);
	if (ImGui::Button("Camera LookAt Reset"))
	{
		camera.eye = { 0,0,2 };
		camera.at = { 0,0,0 };
		camera.up = { 0,1,0 };
	}
	ImGui::Checkbox("Draw Local axes", &local_axes);
	ImGui::Checkbox("Draw World axes", &world_axes);
	if (scene.GetModelCount()) {
		scene.GetModel(0).worldAxes = world_axes;
		scene.GetModel(0).localAxes = local_axes;
	}
	ImGui::Checkbox("Draw Bounding Box", &is_boundingBox);
	scene.draw_box = is_boundingBox;

	ImGui::Checkbox("Draw Normals", &is_normals);
	scene.draw_normals = is_normals;

	static bool DrawFaceNormals = false;
	ImGui::Checkbox("Draw Face Normals", &DrawFaceNormals);
	scene.draw_face_normals = DrawFaceNormals;
	ImGui::Checkbox("Paint Triangles", &scene.paint_triangles);
	ImGui::Checkbox("Gray Scale", &scene.gray_scale);
	ImGui::Checkbox("Color With Buffer", &scene.color_with_buffer);
	if (scene.paint_triangles) { scene.gray_scale = false; scene.color_with_buffer = false; }
	if (scene.gray_scale) { scene.paint_triangles; scene.color_with_buffer = false; }
	if (scene.color_with_buffer) { scene.gray_scale = false; scene.paint_triangles = false; }
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(390, 450));
	ImGui::Begin("Lights Controller");
	static int LightCount = 0;
	static char* lights[5] = { "1","2","3","4","5" };
	ImGui::Text("Point lights = %d", LightCount);
	if (ImGui::Button("Add point lights") && (LightCount < 3))
	{
		LightCount++;

	}
	if (LightCount)
		scene.lighting = true;

	if (LightCount == 1) {
		ImGui::Combo("Choose Light", &LightCount, lights, LightCount);
		ImGui::Text("Light RGB");
		ImGui::ColorEdit3("Ambient", (float*)&scene.GetLight(0).AmbientColor);
		ImGui::ColorEdit3("Diffuse", (float*)&scene.GetLight(0).DiffuseColor);
		ImGui::ColorEdit3("Specular", (float*)&scene.GetLight(0).SpecularColor);
		ImGui::SliderFloat3("Light 1 Translation", &scene.GetLight(0).Translation[3].x, -5.0f, 5.0f);
	}
	ImGui::SliderInt("Alpha", &scene.GetLight(0).alpha, 0, 60);
	if (LightCount == 2) {
		static int light_num = 1;
		ImGui::RadioButton("Light 1", &light_num, 1); ImGui::SameLine();
		ImGui::RadioButton("Light 2", &light_num, 2);
		scene.more_than_1_light = true;

		if (light_num == 1) {
			ImGui::Combo("Choose Light", &LightCount, lights, LightCount);
			ImGui::Text("Light RGB");
			ImGui::ColorEdit3("Ambient", (float*)&scene.GetLight(0).AmbientColor);
			ImGui::ColorEdit3("Diffuse", (float*)&scene.GetLight(0).DiffuseColor);
			ImGui::ColorEdit3("Specular", (float*)&scene.GetLight(0).SpecularColor);
			ImGui::SliderFloat3("Light 1 Translation", &scene.GetLight(0).Translation[3].x, -5.0f, 5.0f);
		}
		else {

			ImGui::Combo("Choose Light", &LightCount, lights, LightCount);
			ImGui::Text("Light RGB");
			ImGui::ColorEdit3("Ambient", (float*)&scene.GetLight(1).AmbientColor);
			ImGui::ColorEdit3("Diffuse", (float*)&scene.GetLight(1).DiffuseColor);
			ImGui::ColorEdit3("Specular", (float*)&scene.GetLight(1).SpecularColor);
			ImGui::SliderFloat3("Light 2 Translation", &scene.GetLight(1).Translation[3].x, -5.0f, 5.0f);
		}


	}
	if (scene.GetModelCount())
	{
		ImGui::Text("Model RGB");
		ImGui::ColorEdit3("Model Ambient", (float*)&scene.GetModel(0).Ka);
		ImGui::ColorEdit3("Model Diffuse", (float*)&scene.GetModel(0).Kd);
		ImGui::ColorEdit3("Model Specular", (float*)&scene.GetModel(0).Ks);
	}

	ImGui::Checkbox("Ambient Shading", &scene.ambient_light); ImGui::SameLine();
	ImGui::Checkbox("Diffuse Lighting", &scene.diffuse_light);
	ImGui::Checkbox("Specular Light", &scene.specular_light);

	ImGui::Checkbox("Reflection Vectors", &scene.reflection_vector);
	ImGui::Checkbox("Fog", &scene.fog);
	ImGui::Checkbox("Blur", &scene.blur);

	static int shading = 0;
	ImGui::RadioButton("Flat Shading", &shading, 1); 

	if (shading == 1) { scene.flat_shading = true; scene.phong = false; }
	if (shading == 2) { scene.flat_shading = false; scene.phong = true; }
	ImGui::Text("Choose Texture");

	static int tex_mapping = 0;
	ImGui::RadioButton("Plane", &tex_mapping, 1); ImGui::SameLine();
	ImGui::RadioButton("Cylinder", &tex_mapping, 2); ImGui::SameLine();
	ImGui::RadioButton("Sphere", &tex_mapping, 3);

	if (tex_mapping == 1) { scene.GetActiveModel().SetPlane(); ImGui::SameLine(); }
	if (tex_mapping == 2) {}
	if (tex_mapping == 3) {}

	ImGui::Checkbox("Texture", &scene.use_texture);
	ImGui::Checkbox("Toon Shading", &scene.toon_shading);
	ImGui::SliderFloat("colors of shades?", &scene.levels, 0, 20);
	ImGui::End();
}