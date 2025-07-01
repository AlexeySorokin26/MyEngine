#pragma once

struct GLFWwindow;

class UIModule {
public:
	static void OnWindowCreate(GLFWwindow* window);
	static void OnWindowClose();
	static void OnWindowUpdateBegin();
	static void OnWindowUpdateDraw();

	static void ShowExampleAppDockSpace(bool* p_open);
};
