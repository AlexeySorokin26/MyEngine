#include "Input.hpp"


bool Input::IsKeyPressed(const KeyCode keyCode)
{
	return KeysPressed[static_cast<size_t>(keyCode)];
}

void Input::PressKey(const KeyCode keyCode)
{
	KeysPressed[static_cast<size_t>(keyCode)] = true;
}

void Input::ReleaseKey(const KeyCode keyCode)
{
	KeysPressed[static_cast<size_t>(keyCode)] = false;
}
