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


