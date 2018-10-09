#include "Window.h"
#include <iostream>

void Window::Setup()
{
	if (!glfwInit())
	{
		MessageBox(NULL, "Failed to initialize GLFW!", "ERROR!", MB_ICONERROR | MB_OK);
		return;
	}

	/*glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);*/

	_window = glfwCreateWindow(_width, _height, title.c_str(), NULL, NULL);
	if (!_window)
	{
		MessageBox(NULL, "Failed to create window!", "ERROR!", MB_ICONERROR | MB_OK);
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		MessageBox(NULL, "Failed to initialize GLAD!", "ERROR!", MB_ICONERROR | MB_OK);
		glfwTerminate();
		return;
	}  

	_resizedCallback = [&](int width, int height)
	{
		_width = width;
		_height = height;
		glViewport(0, 0, width, height);
	};

	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	if (_window)
		glfwDestroyWindow(_window);
	glfwTerminate();
}

std::function<void(int, int)> Window::_resizedCallback;