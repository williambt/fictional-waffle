#pragma once

#include "../Core/GameObject.h"
#include "../Core/Window.h"
#include "../Input/Input.h"
#include "../Physics/PhysicsWorld.h"

class Scene
{
protected:
	std::vector<GameObject*> _gameObjects;
	Window& window;
	
	PhysicsWorld _world;

	glm::vec3 _clearColour = glm::vec3(0.2f, 0.2f, 0.3f);
	double _lastTime = glfwGetTime();
	double _frameTime = 0.0;

	void RemoveGameObject(GameObject* go)
	{
		auto it = std::find(_gameObjects.begin(), _gameObjects.end(), go);
		if (it != _gameObjects.end())
		{
			std::swap(*it, _gameObjects.back());
			_gameObjects.pop_back();
		}
	}

public:

	Scene(Window& window) : window(window) {}
	~Scene() {}

	virtual void Setup() = 0;
	virtual void Update()
	{
		_frameTime = glfwGetTime() - _lastTime;
		_lastTime = glfwGetTime();

		_world.Update(_frameTime);

		Input::Update();
		glfwPollEvents();
	}

	virtual void Draw()
	{
		glClearColor(_clearColour.r, _clearColour.g, _clearColour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (GameObject* go : _gameObjects)
			go->Draw();

		window.SwapBuffers();
	}
};