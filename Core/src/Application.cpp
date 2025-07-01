#include "Application.hpp"
#include "Log.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Input.hpp"

#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/VertexBuffer.hpp"
#include "Rendering/OpenGL/VertexArray.hpp"
#include "Rendering/OpenGL/IndexBuffer.hpp"
#include "Rendering/OpenGL/Renderer.hpp"

#include "Modules/UIModule.hpp"

#include <GLFW/glfw3.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

GLfloat posColors[] = {
	0.0f, -0.5f,  -0.5f, 	 1.0f, 1.0f, 0.0f,
	0.0f,  0.5f,  -0.5f, 	 1.0f, 0.0f, 0.0f,
	0.0f, -0.5f,   0.5f,  	 1.0f, 0.0f, 1.0f,
	0.0f,  0.5f,   0.5f,  	 0.0f, 1.0f, 1.0f
};

GLuint indices[] = {
	0, 1, 2, 3, 2, 1
};

float scale[] = { 1.f, 1.f, 1.f };
float rotate = { 0.f };
float translation[] = { 0.f, 0.f, 1.f };


const char* vertex_shader =
"#version 460\n"
"layout(location=0) in vec3 v_pos;"
"layout(location=1) in vec3 v_col;"
"uniform mat4 modelMat;"
"uniform mat4 proViewMat;"
"out vec3 col;"
"void main() {"
"	col = v_col;"
"	gl_Position = proViewMat * modelMat * vec4(v_pos, 1.0);"
"}";
const char* frag_shader =
"#version 460\n"
"in vec3 col;"
"out vec4 frag_col;"
"void main() {"
"	frag_col = vec4(col, 1.0);" // 4 is alfa 
"}";


std::unique_ptr<ShaderProgram> shaderProgram;

std::unique_ptr<VertexArray> vaoBuffer;
std::unique_ptr<VertexBuffer> posColorsVBO;
std::unique_ptr<IndexBuffer> indexBuffer;

float backgroundCol[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

Application::Application() {
	LOG_INFO("Application started");
}

Application::~Application() {
	LOG_INFO("Application closed");
}

int Application::Start(unsigned int width, unsigned int height, const char* title) {

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

	eventDispather.add_event_listener<EventKeyPressed>(
		[&](EventKeyPressed& event)
		{
			//if (event.keyCode <= KeyCode::KEY_Z) {
			if (event.repeated) {
				LOG_CRIT("Pressed: {0}, repeated", char(event.keyCode));
			}
			else {
				LOG_CRIT("Pressed {0}", char(event.keyCode));
			}
			Input::PressKey(event.keyCode);
			//}
		}
	);
	eventDispather.add_event_listener<EventKeyReleased>(
		[&](EventKeyReleased& event)
		{
			//if (event.keyCode <= KeyCode::KEY_Z) {
			LOG_CRIT("Released {0}", char(event.keyCode));
			Input::ReleaseKey(event.keyCode);
			//}
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

	// Create shader programm 
	{
		shaderProgram = std::make_unique<ShaderProgram>(vertex_shader, frag_shader);
		if (!shaderProgram->IsCompiled()) {
			return false;
		}
	}

	// Fill GPU data and let it know how to handle data
	{
		// Fill GPU with data and explain how to deal with that
		{
			BufferLayout bufferLayout2vec3{ // 2 elements with 3 components
				ShaderDataType::Float3,
				ShaderDataType::Float3
			};
			vaoBuffer = std::make_unique<VertexArray>();
			posColorsVBO = std::make_unique<VertexBuffer>(posColors, sizeof(posColors), bufferLayout2vec3);
			indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
			vaoBuffer->SetIndexBuffer(*indexBuffer);
			vaoBuffer->AddVertexBuffer(*posColorsVBO);
		}
	}

	while (!closedWindow) {
		Renderer::SetClearColor(backgroundCol[0], backgroundCol[1], backgroundCol[2], backgroundCol[3]);
		Renderer::Clear();

		// use this shader prog
		shaderProgram->Bind();

		glm::mat4 scaleMat(
			scale[0], 0, 0, 0,
			0, scale[1], 0, 0,
			0, 0, scale[2], 0,
			0, 0, 0, 1
		);

		float rotateRad = glm::radians(rotate);
		glm::mat4 rotateZMat(
			cos(rotateRad), sin(rotateRad), 0, 0,
			-sin(rotateRad), cos(rotateRad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		glm::mat4 translationMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translation[0], translation[1], translation[2], 1
		);

		auto modelMat = translationMat * rotateZMat * scaleMat;
		shaderProgram->SetMatrix4("modelMat", modelMat);

		camera.SetProjMode(perspectiveCam ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

		auto projViewMat = camera.GetProjMatrix() * camera.GetViewMatrix();
		shaderProgram->SetMatrix4("proViewMat", projViewMat);

		// draw
		Renderer::Draw(*vaoBuffer);

		// Create a window to adjust params
		{
			// Create a frame where we want to draw
			UIModule::OnWindowUpdateBegin();
			bool show = true;
			UIModule::ShowExampleAppDockSpace(&show);
			ImGui::ShowDemoWindow();
			ImGui::Begin("Adjust parameters");
			ImGui::ColorEdit4("Change background color", backgroundCol);
			ImGui::SliderFloat3("Scale mat", scale, 0.f, 1.f);
			ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
			ImGui::SliderFloat3("Translation", translation, 0.f, 10.f);
			ImGui::End();

			OnUiDraw();

			UIModule::OnWindowUpdateDraw();
		}

		window->on_update();
		OnUpdate();
	}

	return 0;
}

void Application::OnUpdate() {

}

void Application::OnUiDraw() {

}
