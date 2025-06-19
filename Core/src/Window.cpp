#include "Window.hpp"
#include "Log.hpp"
#include "Camera.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/VertexBuffer.hpp"
#include "Rendering/OpenGL/VertexArray.hpp"
#include "Rendering/OpenGL/IndexBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include <memory>

static bool glfw_initialized = false;

GLfloat posColors[] = {
	-0.5f, -0.5f, 0.0f,	 1.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,	 1.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f,	 0.0f, 1.0f, 1.0f
};

GLuint indices[] = {
	0, 1, 2, 3, 2, 1
};

float scale[] = { 1.f, 1.f, 1.f };
float rotate = { 0.f };
float translation[] = { 0.f, 0.f, 1.f };

float camPos[] = { 0.f, 0.f, 10.f };
float camRotation[] = { 0.f, 0.f, 0.f };
bool perspectiveCam = false;
Camera camera;

const char* vertex_shader =
"#version 460\n"
"layout(location=0) in vec3 v_pos;"
"layout(location=1) in vec3 v_col;"
"uniform mat4 modelMat;"
"uniform mat4 proViewMat;"
"out vec3 col;"
"void main() {"
"	col = v_col;"
"	gl_Position = proViewMat * modelMat * vec4(v_pos, 1.0);"
"}";
const char* frag_shader =
"#version 460\n"
"in vec3 col;"
"out vec4 frag_col;"
"void main() {"
"	frag_col = vec4(col, 1.0);" // 4 is alfa 
"}";


std::unique_ptr<ShaderProgram> shaderProgram;

std::unique_ptr<VertexArray> vaoBuffer;
std::unique_ptr<VertexBuffer> posColorsVBO;
std::unique_ptr<IndexBuffer> indexBuffer;

Window::Window(std::string title, const unsigned int width, unsigned int height) :
	windowData({ std::move(title), width, height }) {
	int res = init();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

Window::~Window() {
	shutdown();
}

int Window::init() {
	LOG_INFO("Window creation");
	// Init GLFW
	{
		/* Initialize the library */
		if (!glfw_initialized) {
			if (!glfwInit()) {
				LOG_CRIT("glfw was not init");
				return -1;
			}
			glfw_initialized = true;
		}

	}

	/* Create a windowed mode window and its OpenGL context */
	{
		window = glfwCreateWindow(windowData.width, windowData.height, windowData.title.c_str(), NULL, NULL);
		if (!window) {
			LOG_CRIT("window was not init");
			glfwTerminate();
			return -1;
		}
		/* Make the window's context current */
		glfwMakeContextCurrent(window);
	}

	// Iini GLAD
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_CRIT("Failed to initialize GLAD");
			return -1;
		}
	}

	// Set Callbacks
	{
		// Set pointer in window to have windowData to use it in callbacks since we cannot pass it 
		// why not pass in []?
		glfwSetWindowUserPointer(window, &windowData);
		// here we got event from window (our app not application.cpp)
		glfwSetWindowSizeCallback(window,
			[](GLFWwindow* window, int width, int height) {
				//LOG_INFO("[Window] New size width {0} and height {1} ", width, height);

				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.height = height;
				data.width = width;
				// Create event
				EventWindowResize event(width, height);
				// Call callback this guy window->set_event_callback or more precise this one eventDispather.dispatch(e);
				data.eventCallbackFn(event);
			}
		);

		glfwSetCursorPosCallback(window,
			[](GLFWwindow* window, double x, double y) {
				//LOG_INFO("[Callback from Window] Cursor pos {0}X{1}", x, y);

				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventMouseMoved event(x, y);
				data.eventCallbackFn(event);
			}
		);

		glfwSetWindowCloseCallback(window,
			[](GLFWwindow* window) {
				//LOG_INFO("[Callback from Window] Window closed");

				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventWindowClose event;
				data.eventCallbackFn(event);
			}
		);

		// different from glfwSetWindowSizeCallback that we got size in pixels here
		glfwSetFramebufferSizeCallback(window,
			[](GLFWwindow* window, int width, int height) {
				glViewport(0, 0, width, height);
			}
		);
	}

	// Create shader programm 
	{
		shaderProgram = std::make_unique<ShaderProgram>(vertex_shader, frag_shader);
		if (!shaderProgram->IsCompiled()) {
			return false;
		}
	}

	// Fill GPU data and let it know how to handle data
	{
		// Fill GPU with data and explain how to deal with that
		{
			BufferLayout bufferLayout2vec3{ // 2 elements with 3 components
				ShaderDataType::Float3,
				ShaderDataType::Float3
			};
			vaoBuffer = std::make_unique<VertexArray>();
			posColorsVBO = std::make_unique<VertexBuffer>(posColors, sizeof(posColors), bufferLayout2vec3);
			indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
			vaoBuffer->SetIndexBuffer(*indexBuffer);
			vaoBuffer->AddVertexBuffer(*posColorsVBO);
		}
	}

	glm::mat3 mat1{ 4, 0,0, 1, 1, 1 ,2 ,2 , 2 };

	return 0;
}

void Window::shutdown() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::on_update() {
	glClearColor(backgroundCol[0], backgroundCol[1], backgroundCol[2], backgroundCol[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	{
		// let imgut know size of our window
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());
		// Create a frame where we want to draw
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// use this shader prog
		shaderProgram->Bind();

		glm::mat4 scaleMat(
			scale[0], 0, 0, 0,
			0, scale[1], 0, 0,
			0, 0, scale[2], 0,
			0, 0, 0, 1
		);

		float rotateRad = glm::radians(rotate);
		glm::mat4 rotateZMat(
			cos(rotateRad), sin(rotateRad), 0, 0,
			-sin(rotateRad), cos(rotateRad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		glm::mat4 translationMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translation[0], translation[1], translation[2], 1
		);
		// Create a window to adjust params
		{
			ImGui::Begin("Adjust parameters");
			ImGui::ColorEdit4("Change background color", backgroundCol);
			ImGui::SliderFloat3("Scale mat", scale, 0.f, 1.f);
			ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
			ImGui::SliderFloat3("Translation", translation, 0.f, 10.f);

			ImGui::SliderFloat3("Cam pos", camPos, -10.f, 10.f);
			ImGui::SliderFloat3("Cam rotation", camRotation, 0.f, 360.f);
			ImGui::Checkbox("Cam perspective", &perspectiveCam);

			ImGui::End();
		}

		auto modelMat = translationMat * rotateZMat * scaleMat;

		shaderProgram->SetMatrix4("modelMat", modelMat);
		camera.SetPosRot(glm::vec3(camPos[0], camPos[1], camPos[2]), glm::vec3(camRotation[0], camRotation[1], camRotation[2]));
		camera.SetProjMode(perspectiveCam ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
		auto projViewMat = camera.GetProjMatrix() * camera.GetViewMatrix();
		shaderProgram->SetMatrix4("proViewMat", projViewMat);

		vaoBuffer->Bind();
		// draw
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vaoBuffer->GetIndicesCount()), GL_UNSIGNED_INT, nullptr);

		// Conver UI data into data to draw
		// create here vertex buffers index buffers and so on
		// so data for opengl for example 
		ImGui::Render();
		// Use this data in our backend (opengl)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


	glfwSwapBuffers(window);
	glfwPollEvents();
}


