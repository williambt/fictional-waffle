#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include <functional>
#include "glm/vec2.hpp"

struct Rect
{
	float left;
	float top;
	float right;
	float bottom;
};

class Window
{
private:

	GLFWwindow* _window;
	int _width, _height;
	std::string title;

	void Setup();

	static std::function<void(int, int)> _resizedCallback;
	static void resizedCallback(GLFWwindow* window, int width, int height)
	{
		_resizedCallback(width, height);
	}

	Rect _worldBounds;

public:

	Window(int width, int height, const char* title) : _width(width), _height(height), title(title), _worldBounds({ 0, 0, 0, 0 })
	{
		Setup();
	}
	Window(int width, int height, const std::string &title) : _width(width), _height(height), title(title), _worldBounds({ 0, 0, 0, 0 })
	{
		Setup();
	}
	~Window();

	void SetWorldBounds(Rect bounds) { _worldBounds = { bounds.left * GetRatio(), bounds.top, bounds.right * GetRatio(), bounds.bottom }; }
	Rect GetWorldBounds() const { return _worldBounds; }

	glm::vec2 ScreenToWorld(glm::vec2 screenCoord) const 
	{
		return glm::vec2
		(
			screenCoord.x / _width * (_worldBounds.right - _worldBounds.left) + _worldBounds.left,
			-(screenCoord.y / _height * (_worldBounds.top - _worldBounds.bottom) + _worldBounds.bottom)
		);
	}

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

	float GetRatio() const { return _width > _height ? (float)_width / (float)_height : (float)_height / (float)_width; }

	bool IsOpen() const { return !glfwWindowShouldClose(_window); }
	void Close() { glfwSetWindowShouldClose(_window, GLFW_TRUE); }
	void SwapBuffers() { glfwSwapBuffers(_window); }

	operator GLFWwindow*() { return _window; }
	GLFWwindow* GetGLFWwindow() { return _window; }
};
