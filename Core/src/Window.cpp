#include "Window.hpp"
#include "Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static bool glfw_initialized = false;

Window::Window(std::string title, const unsigned int width, unsigned int height) :
	windowData({ std::move(title), width, height }) {
	int res = init();
}

Window::~Window() {
	shutdown();
}

int Window::init() {
	LOG_INFO("Window creation");
	/* Initialize the library */
	if (!glfw_initialized) {
		if (!glfwInit()) {
			LOG_CRIT("glfw was not init");
			return -1;
		}
		glfw_initialized = true;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(windowData.width, windowData.height, windowData.title.c_str(), NULL, NULL);
	if (!window) {
		LOG_CRIT("window was not init");
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_CRIT("Failed to initialize GLAD");
		return -1;
	}
	glfwSetWindowUserPointer(window, &windowData);
	glfwSetWindowSizeCallback(window,
		[](GLFWwindow* window, int width, int height) {
			LOG_INFO("[Window] New size width {0} and height {1} ", width, height);

			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.height = height;
			data.width = width;

			Event event;
			event.height = height;
			event.width = width;

			data.eventCallbackFn(event);
		}
	);

	return 0;
}

void Window::shutdown() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::on_update() {
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	glfwPollEvents();
}


