#pragma once

#include "Event.hpp"
#include "Camera.hpp"

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

	virtual int Start(unsigned int width, unsigned int height, const char* title);
	virtual void OnUpdate();
	virtual void OnUiDraw();

	float camPos[3] = { 0.f, 0.f, 10.f };
	float camRotation[3] = { 0.f, 0.f, 0.f };
	bool perspectiveCam = true;
	Camera camera{ glm::vec3(-5, 0, 0) };

private:
	std::unique_ptr<Window> window;

	EventDispatcher eventDispather;
	bool closedWindow = false;
};