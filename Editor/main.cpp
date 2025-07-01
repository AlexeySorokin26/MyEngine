#include <iostream>
#include <memory>

#include "Application.hpp"
#include "Input.hpp"
#include <imgui/imgui.h>

class Editor : public Application {
public:
	void OnUpdate() override {

		bool moveCam = false;

		glm::vec3 movementDelta{ 0,0,0 };
		glm::vec3 rotationDelta{ 0,0,0 };

		if (Input::IsKeyPressed(KeyCode::KEY_W)) {
			movementDelta.x += 0.05f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_S)) {
			movementDelta.x -= 0.05f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_A)) {
			movementDelta.y -= 0.05f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_D)) {
			movementDelta.y += 0.05f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_E)) {
			movementDelta.z += 0.05f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_Q)) {
			movementDelta.z -= 0.05f;
			moveCam = true;
		}

		if (Input::IsKeyPressed(KeyCode::KEY_UP)) {
			rotationDelta.y -= 0.5f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_DOWN)) {
			rotationDelta.y += 0.5f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_RIGHT)) {
			rotationDelta.z += 0.5f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_LEFT)) {
			rotationDelta.z -= 0.5f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_P)) {
			rotationDelta.x += 0.5f;
			moveCam = true;
		}
		if (Input::IsKeyPressed(KeyCode::KEY_O)) {
			rotationDelta.x -= 0.5f;
			moveCam = true;
		}

		if(moveCam)
			camera.AddMovementAndRotation(movementDelta, rotationDelta);
	}
	void OnUiDraw() override {
		camPos[0] = camera.GetCamPos().x;
		camPos[1] = camera.GetCamPos().y;
		camPos[2] = camera.GetCamPos().z;
		camRotation[0] = camera.GetCamRotation().x;
		camRotation[1] = camera.GetCamRotation().y;
		camRotation[2] = camera.GetCamRotation().z;

		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("Cam pos", camPos, -10.f, 10.f)) {
			camera.SetPos(glm::vec3(camPos[0], camPos[1], camPos[2]));
		}
		if (ImGui::SliderFloat3("Cam rotation", camRotation, 0.f, 360.f)) {
			camera.SetRotation(glm::vec3(camRotation[0], camRotation[1], camRotation[2]));
		}
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