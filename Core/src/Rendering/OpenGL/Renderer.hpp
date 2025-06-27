#pragma once

struct GLFWwindow;

class VertexArray;

class Renderer {
public:
	static bool Init(GLFWwindow* w);
	static void Draw(const VertexArray& va);
	static void SetClearColor(const float r, const float g, const float b, const float a);
	static void Clear();
	static void SetViewport(const unsigned w, const unsigned h, const unsigned leftOffset = 0, const unsigned bottomOffset = 0);
	static const char* GetVendorStr();
	static const char* GetRendererStr();
	static const char* GetVersionStr();
};