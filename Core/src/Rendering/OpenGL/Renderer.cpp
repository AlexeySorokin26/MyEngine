#include "Renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "Log.hpp"

bool Renderer::Init(GLFWwindow* w)
{
	/* Make the window's context current */
	glfwMakeContextCurrent(w);

	// Init GLAD
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_CRIT("Failed to initialize GLAD");
			return false;
		}
	}

	LOG_INFO("OpenGL context initialized:");
	LOG_INFO("	Vendor: {0}", GetVendorStr());
	LOG_INFO("	Renderer: {0}", GetRendererStr());
	LOG_INFO("	Version: {0}", GetVersionStr());

	return true;
}

void Renderer::Draw(const VertexArray& va)
{
	va.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(va.GetIndicesCount()), GL_UNSIGNED_INT, nullptr);
}

void Renderer::SetClearColor(const float r, const float g, const float b, const float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetViewport(const unsigned w, const unsigned h, const unsigned leftOffset, const unsigned bottomOffset)
{
	glViewport(leftOffset, bottomOffset, w, h);
}

const char* Renderer::GetVendorStr()
{
	return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

const char* Renderer::GetRendererStr()
{
	return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

const char* Renderer::GetVersionStr()
{
	return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}