#include <iostream>
#include <memory>

#include "Application.hpp"
#include "Input.hpp"
#include <imgui/imgui.h>

class Editor : public Application {
public:
	void OnUpdate() override {
		if (Input::IsKeyPressed(KeyCode::KEY_W)) {
			camPos[2] -= 0.1;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_S)) {
			camPos[2] += 0.1;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_A)) {
			camPos[0] += 0.1;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_D)) {
			camPos[0] -= 0.1;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_E)) {
			camPos[1] += 0.1;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_Q)) {
			camPos[1] -= 0.1;
		}

		if (Input::IsKeyPressed(KeyCode::KEY_UP)) {
			camRotation[0] += 0.5;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_DOWN)) {
			camRotation[0] -= 0.5;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_RIGHT)) {
			camRotation[1] += 0.5;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_LEFT )) {
			camRotation[1] -= 0.5;
		}
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