#include "../Core/Window.h"
#include "Input.h"

void Input::KeyCallback(GLFWwindow * window, int key, int code, int action, int mods)
{
	if (action == GLFW_PRESS)
		_keys[key] = KEY_PRESSED;
	else if (action == GLFW_RELEASE)
		_keys[key] = KEY_RELEASED;
}

void Input::MButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		_mButtons[button] = KEY_PRESSED;
	else if (action == GLFW_RELEASE)
		_mButtons[button] = KEY_RELEASED;
}

void Input::MPosCallback(GLFWwindow * window, double xpos, double ypos)
{
	_mousePos = glm::vec2(xpos, ypos);
}

void Input::ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	_scroll = (float)yoffset;
}

bool Input::GetKeyPressed(int key)
{
	return _keys[key] == KEY_PRESSED;
}

bool Input::GetKeyReleased(int key)
{
	return _keys[key] == KEY_RELEASED;
}

bool Input::GetKeyDown(int key)
{
	return _keys[key] == KEY_PRESSED || _keys[key] == KEY_DOWN;
}

bool Input::GetMButtonPressed(int button)
{
	return _mButtons[button] == KEY_PRESSED;
}

bool Input::GetMButtonReleased(int button)
{
	return _mButtons[button] == KEY_RELEASED;
}

bool Input::GetMButtonDown(int button)
{
	return _mButtons[button] == KEY_PRESSED || _mButtons[button] == KEY_DOWN;
}

void Input::SetupCallbacks(Window* window)
{
	_window = window;
	glfwSetKeyCallback(window->GetGLFWwindow(), KeyCallback);
	glfwSetMouseButtonCallback(window->GetGLFWwindow(), MButtonCallback);
	glfwSetCursorPosCallback(window->GetGLFWwindow(), MPosCallback);
	glfwSetScrollCallback(window->GetGLFWwindow(), ScrollCallback);
}

void Input::Update()
{
	for (int i = 0; i < GLFW_KEY_LAST; ++i)
	{
		if (_keys[i] == KEY_PRESSED)
			_keys[i] = KEY_DOWN;
		else if (_keys[i] == KEY_RELEASED)
			_keys[i] = KEY_UP;
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i)
	{
		if (_mButtons[i] == KEY_PRESSED)
			_mButtons[i] = KEY_DOWN;
		else if (_mButtons[i] == KEY_RELEASED)
			_mButtons[i] = KEY_UP;
	}

	_lastMousePos = _mousePos;
	_scroll = 0;
}

unsigned char Input::_keys[GLFW_KEY_LAST];
unsigned char Input::_mButtons[GLFW_MOUSE_BUTTON_LAST];
glm::vec2 Input::_mousePos;
glm::vec2 Input::_lastMousePos;
float Input::_scroll;
enum Input::KeyState;
Window* Input::_window = nullptr;