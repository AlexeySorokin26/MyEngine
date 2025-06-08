#pragma once

#include "Event.hpp"

#include <memory>

class Window;

class Application {
public:
	Application();
	virtual ~Application();

	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	virtual int start(unsigned int width, unsigned int height, const char* title);
	virtual void on_update();
private:
	std::unique_ptr<Window> window;

	EventDispatcher eventDispather;
	bool closedWindow = false;
};