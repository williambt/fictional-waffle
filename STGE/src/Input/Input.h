#pragma once

#include <glm/vec2.hpp>
class Window;

class Input
{
private:

	static Window* _window;

	static unsigned char _keys[];
	static unsigned char _mButtons[];

	static glm::vec2 _mousePos;
	static glm::vec2 _lastMousePos;
	static float _scroll;
	
	static void KeyCallback(GLFWwindow* window, int key, int code, int action, int mods);
	static void MButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
	enum KeyState { KEY_UP, KEY_PRESSED, KEY_DOWN, KEY_RELEASED };

	static inline unsigned char GetKeyState(int key) { return _keys[key]; }
	static inline unsigned char MButtonState(int button) { return _mButtons[button]; }
	
	static bool GetKeyPressed(int key);
	static bool GetKeyReleased(int key);
	static bool GetKeyDown(int key);
	static bool GetMButtonPressed(int button);
	static bool GetMButtonReleased(int button);
	static bool GetMButtonDown(int button);
	 
	static inline float GetScroll() { return _scroll; }

	static inline glm::vec2 GetMousePos() { return _mousePos; }
	static inline glm::vec2 GetMouseMovement() { return _mousePos - _lastMousePos; }

	static inline void LockCursor() { glfwSetInputMode(_window->GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
	static inline void UnlockCursor() { glfwSetInputMode(_window->GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	static void SetupCallbacks(Window* window);
	static void Update();
};

