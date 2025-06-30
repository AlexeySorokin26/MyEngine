#pragma once

#include "Keys.hpp"

class Input {
public:
	static bool IsKeyPressed(const KeyCode keyCode);
	static void PressKey(const KeyCode keyCode);
	static void ReleaseKey(const KeyCode keyCode);
private:
	inline static bool KeysPressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};
};