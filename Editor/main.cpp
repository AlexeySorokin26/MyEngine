#include <iostream>
#include <memory>

#include "Application.hpp"

class MyApplication : public Application {
public:
	void on_update() override {
		std::cout << "Updating application..." << std::endl;
	}
};

int main() {

	auto app = std::make_unique<MyApplication>();
	int result = app->start(800, 600, "Hello World");
	if (result != 0) {
		std::cerr << "Failed to start application" << std::endl;
		return result;
	}
	return 0;
}