#include "Application.hpp"
#include "Log.hpp"
#include "Window.hpp"

Application::Application() {
	LOG_INFO("Application started");
}

Application::~Application() {
	LOG_INFO("Application closed");
}

int Application::start(unsigned int width, unsigned int height, const char* title) {

	window = std::make_unique<Window>(title, width, height);
	// we set callback in dipatcher
	eventDispather.add_event_listener<EventMouseMoved>(
		[](EventMouseMoved& event) {
			//LOG_INFO("[MouseMoved] x {0} and y {1} ", event.x, event.y);
		}
	);
	// we set another callback in dipatcher
	eventDispather.add_event_listener<EventWindowResize>(
		[](EventWindowResize& event) {
			//LOG_INFO("[MouseMoved] width {0} and height {1} ", event.width, event.height);
		}
	);
	// one more 
	eventDispather.add_event_listener<EventWindowClose>(
		[&](EventWindowClose& event) {
			closedWindow = true;
		}
	);
	// now we need to call callbacks somehow
	// here how function dipatch will call them
	// we call this function everytime we have some callback
	window->set_event_callback(
		[&](BaseEvent& e) {
			eventDispather.dispatch(e);
		}
	);

	while (!closedWindow) {
		window->on_update();
		on_update();
	}

	return 0;
}

void Application::on_update() {

}
