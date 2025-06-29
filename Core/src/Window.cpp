#include "Window.hpp"
#include "Log.hpp"

#include "Modules/UIModule.hpp"
#include <GLFW/glfw3.h>

#include "Rendering/OpenGL/Renderer.hpp"

#include <memory>

Window::Window(std::string title, const unsigned int width, unsigned int height) :
	windowData({ std::move(title), width, height }) {
	int res = init();
}

Window::~Window() {
	shutdown();
}

int Window::init() {
	LOG_INFO("Window creation");

	glfwSetErrorCallback([](int errorCode, const char* desc)
		{
			LOG_CRIT("GLFW error: {0}", desc);
		}
	);

	// Init GLFW
	{
		/* Initialize the library */
		if (!glfwInit()) {
			LOG_CRIT("glfw was not init");
			return -1;
		}
	}

	/* Create a windowed mode window and its OpenGL context */
	{
		window = glfwCreateWindow(windowData.width, windowData.height, windowData.title.c_str(), NULL, NULL);
		if (!window) {
			LOG_CRIT("window was not init");
			return -2;
		}
	}

	if (!Renderer::Init(window)) {
		LOG_CRIT("Failed to init OpenGL");
		return -3;
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
				Renderer::SetViewport(width, height);
			}
		);
	}

	// ImGUI
	{
		UIModule::OnWindowCreate(window);
	}

	return 0;
}

void Window::shutdown() {
	UIModule::OnWindowClose();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::on_update() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}


