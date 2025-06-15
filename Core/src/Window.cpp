#include "Window.hpp"
#include "Log.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/VertexBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <memory>

static bool glfw_initialized = false;

GLfloat points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
};
GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
};

const char* vertex_shader =
"#version 460\n"
"layout(location=0) in vec3 v_pos;"
"layout(location=1) in vec3 v_col;"
"out vec3 col;"
"void main() {"
"	col = v_col;"
"	gl_Position = vec4(v_pos, 1.0);"
"}";
const char* frag_shader =
"#version 460\n"
"in vec3 col;"
"out vec4 frag_col;"
"void main() {"
"	frag_col = vec4(col, 1.0);" // 4 is alfa 
"}";

GLuint vao;

std::unique_ptr<ShaderProgram> shaderProgram;
std::unique_ptr<VertexBuffer> pointsVBO;
std::unique_ptr<VertexBuffer> colorsVBO;

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
				LOG_INFO("[Window] New size width {0} and height {1} ", width, height);

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
				LOG_INFO("[Callback from Window] Cursor pos {0}X{1}", x, y);

				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventMouseMoved event(x, y);
				data.eventCallbackFn(event);
			}
		);

		glfwSetWindowCloseCallback(window,
			[](GLFWwindow* window) {
				LOG_INFO("[Callback from Window] Window closed");

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
		pointsVBO = std::make_unique<VertexBuffer>(points, sizeof(points));
		colorsVBO = std::make_unique<VertexBuffer>(colors, sizeof(colors));
		// Explain GPU how to handle data
		// Create VAO->activate it->activate interesting pos and relevant vbo->link data
		glGenVertexArrays(1, &vao);
		// make it current
		glBindVertexArray(vao);
		// activate pos (by default they switchted off)
		glEnableVertexAttribArray(0);

		// activate correct buffer obj
		pointsVBO->Bind();
		// link data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// the same for colors
		glEnableVertexAttribArray(1);
		colorsVBO->Bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	return 0;
}

void Window::shutdown() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::on_update() {
	glClearColor(backgroundCol[0], backgroundCol[1], backgroundCol[2], backgroundCol[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	// use this shader prog
	shaderProgram->Bind();
	// use this vao
	glBindVertexArray(vao);
	// draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// IMgui part
	{
		// let imgut know size of our window
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());
		// Create a frame where we want to draw
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Draw (just create a demo window)
		ImGui::ShowDemoWindow();

		// Create a window to change color
		{
			ImGui::Begin("Backroungd color window");
			ImGui::ColorEdit4("Change background color", backgroundCol);
			ImGui::End();
		}

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


