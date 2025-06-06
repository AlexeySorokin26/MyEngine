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
	window->set_event_callback(
		[](Event& event) {
			LOG_INFO("[Event] width {0} and height {1} ", event.width, event.height);
		}
	);

	while (true) {
		window->on_update();
		on_update();
	}

	return 0;
}

void Application::on_update() {

}
