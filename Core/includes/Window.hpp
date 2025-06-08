#pragma once

#include "Event.hpp"

#include <string>
#include <functional>

class GLFWwindow;

class Window {
public:
	using EventCallbackFn = std::function<void(BaseEvent&)>;

	Window(std::string title, const unsigned int width, unsigned int height);
	~Window();

	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	void on_update();
	unsigned int get_width() const { return windowData.width; };
	unsigned int get_height() const { return windowData.height; };

	void set_event_callback(const EventCallbackFn& callback) { windowData.eventCallbackFn = callback; }
private:

	struct WindowData {
		std::string title;
		unsigned int width;
		unsigned int height;
		EventCallbackFn eventCallbackFn;
	};

	int init();
	void shutdown();

	GLFWwindow* window;
	WindowData windowData;
};