#include <iostream>
#include <memory>

#include "Application.hpp"
#include <imgui/imgui.h>

class Editor : public Application {
public:
	void OnUpdate() override {
		//std::cout << "Updating application..." << std::endl;
	}
	void OnUiDraw() override {
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("Cam pos", camPos, -10.f, 10.f);
		ImGui::SliderFloat3("Cam rotation", camRotation, 0.f, 360.f);
		ImGui::Checkbox("Cam perspective", &perspectiveCam);
		ImGui::End();
	}
};

int main() {

	auto app = std::make_unique<Editor>();
	int result = app->Start(800, 600, "Hello World");
	if (result != 0) {
		std::cerr << "Failed to start application" << std::endl;
		return result;
	}
	return 0;
}