#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
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
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
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

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	//Task - GUI Window
    {
        static glm::mat4 local_scale = {
                            glm::vec4(1.0f,0.0f,0.0f,0.0f),
                            glm::vec4(0.0f,1.0f,0.0f,0.0f),
                            glm::vec4(0.0f,0.0f,1.0f,0.0f),
                            glm::vec4(0.0f,0.0f,0.0f,1.0f)
        };
        static glm::mat4 local_translate = {
                                glm::vec4(1.0f,0.0f,0.0f,0.0f),
                                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                                glm::vec4(0.0f,0.0f,1.0f,0.0f),
                                glm::vec4(0.0f,0.0f,0.0f,1.0f)
        };
        static glm::mat4 local_rotate = {
                                glm::vec4(1.0f,0.0f,0.0f,0.0f),
                                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                                glm::vec4(0.0f,0.0f,1.0f,0.0f),
                                glm::vec4(0.0f,0.0f,0.0f,1.0f)
        };
        static glm::mat4 world_scale = {
                                glm::vec4(1.0f,0.0f,0.0f,0.0f),
                                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                                glm::vec4(0.0f,0.0f,1.0f,0.0f),
                                glm::vec4(0.0f,0.0f,0.0f,1.0f)
        };
        static glm::mat4 world_translate = {
                                glm::vec4(1.0f,0.0f,0.0f,0.0f),
                                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                                glm::vec4(0.0f,0.0f,1.0f,0.0f),
                                glm::vec4(0.0f,0.0f,0.0f,1.0f)
        };
        static glm::mat4 world_rotate = {
                                glm::vec4(1.0f,0.0f,0.0f,0.0f),
                                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                                glm::vec4(0.0f,0.0f,1.0f,0.0f),
                                glm::vec4(0.0f,0.0f,0.0f,1.0f)
        };
        static bool mode = true;
        static float localscale[] = { 0.0f, 0.0f };
        static float worldscale[] = { 0.0f, 0.0f };
        static float localtranslate[] = { 0.0f, 0.0f };
        static float worldtranslate[] = { 0.0f, 0.0f };
        static float local_x = 0.0f;
        static float local_y = 0.0f;
        static float local_Z = 0.0f;
        static float world_x = 0.0f;
        static float world_y = 0.0f;
        static float world_z = 0.0f;
        static glm::vec3 color(0.0f, 0.0f, 0.0f);
        ImGui::Begin("Edit Model");    
        ImGui::Text("User Transformation control");
        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Local Transform"))
            {
                mode = true;
                ImGui::SliderFloat2("scale", localscale, -1.0f, 1.0f);
                local_scale = {
                        glm::vec4(localscale[0] + 1,0.0f,0.0f,0.0f),
                        glm::vec4(0.0f,localscale[1] + 1,0.0f,0.0f),
                        glm::vec4(0.0f,0.0f,1.0f,0.0f),
                        glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                ImGui::SliderFloat2("Translate", localtranslate, -1.0f, 1.0f);
                local_translate = {
                        glm::vec4(1.0f,0.0f,0.0f,0.0f),
                        glm::vec4(0.0f,1.0f,0.0f,0.0f),
                        glm::vec4(0.0f,0.0f,1.0f,0.0f),
                        glm::vec4(localtranslate[0] * 600,localtranslate[1] * 350,0.0f,1.0f)
                };
                ImGui::SliderFloat("X rotate", &local_x, 0, 2 * M_PI);
                ImGui::SliderFloat("Y rotate", &local_y, 0, 2 * M_PI);
                ImGui::SliderFloat("Z rotate", &local_Z, 0, 2 * M_PI);
                glm::mat4 rotate_x_mat = {
                glm::vec4(1.0f,0.0f,0.0f,0.0f),
                glm::vec4(0.0f,cos(local_x),sin(local_x),0.0f),
                glm::vec4(0.0f,-sin(local_x),cos(local_x),0.0f),
                glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                glm::mat4 rotate_y_mat = {
                glm::vec4(cos(local_y),0.0f,sin(local_y),0.0f),
                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                glm::vec4(-sin(local_y),0.0f,cos(local_y),0.0f),
                glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                glm::mat4 rotate_z_mat = {
                glm::vec4(cos(local_Z),sin(local_Z),0.0f,0.0f),
                glm::vec4(-sin(local_Z),cos(local_Z),0.0f,0.0f),
                glm::vec4(0.0f,0.0f,1.0f,0.0f),
                glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                local_rotate = rotate_z_mat * rotate_y_mat * rotate_x_mat;
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("World Transform"))
            {
                mode = false;
                ImGui::SliderFloat2("scale", worldscale, -1.0f, 1.0f);
                world_scale = {
                        glm::vec4(worldscale[0] + 1,0.0f,0.0f,0.0f),
                        glm::vec4(0.0f,worldscale[1] + 1,0.0f,0.0f),
                        glm::vec4(0.0f,0.0f,1.0f,0.0f),
                        glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                ImGui::SliderFloat2("Translate", worldtranslate, -1.0f, 1.0f);
                world_translate = {
                        glm::vec4(1.0f,0.0f,0.0f,0.0f),
                        glm::vec4(0.0f,1.0f,0.0f,0.0f),
                        glm::vec4(0.0f,0.0f,1.0f,0.0f),
                        glm::vec4(worldtranslate[0] * 600,worldtranslate[1] * 350,0.0f,1.0f)
                };
                ImGui::SliderFloat("X rotate", &world_x, 0, 2 * M_PI);
                ImGui::SliderFloat("Y rotate", &world_y, 0, 2 * M_PI);
                ImGui::SliderFloat("Z rotate", &world_z, 0, 2 * M_PI);
                glm::mat4 rotate_x_mat = {
                glm::vec4(1.0f,0.0f,0.0f,0.0f),
                glm::vec4(0.0f,cos(world_x),sin(world_x),0.0f),
                glm::vec4(0.0f,-sin(world_x),cos(world_x),0.0f),
                glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                glm::mat4 rotate_y_mat = {
                glm::vec4(cos(world_y),0.0f,sin(world_y),0.0f),
                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                glm::vec4(-sin(world_y),0.0f,cos(world_y),0.0f),
                glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                glm::mat4 rotate_z_mat = {
                glm::vec4(cos(world_z),sin(world_z),0.0f,0.0f),
                glm::vec4(-sin(world_z),cos(world_z),0.0f,0.0f),
                glm::vec4(0.0f,0.0f,1.0f,0.0f),
                glm::vec4(0.0f,0.0f,0.0f,1.0f)
                };
                world_rotate = rotate_z_mat * rotate_y_mat * rotate_x_mat; 
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::ColorEdit3("color", (float*)&color);
        if (scene.GetModelCount() > 0) 
        {
            MeshModel& model = scene.GetActiveModel();
            model.SetIsLocal(mode);
            model.SetColor(color);
            if (mode == true) //Local 
            {
                model.SetLocalScale(local_scale);
                model.SetLocalTranslate(local_translate);
                model.SetLocalRotate(local_rotate);
            }
        	if (mode == false) // World
            {
                model.SetWorldScale(world_scale);
                model.SetWorldTransform(world_translate);
                model.SetWorldRotate(world_rotate);
            }
        }
        ImGui::End();
    }
}