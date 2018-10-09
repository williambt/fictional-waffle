#pragma once

//#include "Core/GameObject.h"
#include <fstream>
#include <vector>
#include "Physics/PhysicsWorld.h"
#include <string>
#include <sstream>
#include "Rendering/Shapes/Quad.h"

static std::vector<GameObject*> readMLO(std::string fileName, PhysicsWorld& pWorld)
{
	std::vector<GameObject*> res;
	std::ifstream file;
	file.open(fileName, std::ifstream::in);

	if (!file.is_open())
	{
		std::cout << "Failed to open \"" << fileName << "\"!" << std::endl;
	}
	
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "ground")
		{
			glm::vec2 p1, p2;
			ss >> p1.x >> p1.y >> p2.x >> p2.y;

			GameObject* go = new GameObject();
			go->AddShape(new Quad(), glm::vec3(0), 0, glm::vec2(p2.x - p1.x, p1.y - p2.y));
			go->SetPosition(glm::vec3(p1.x + ((p2.x - p1.x) / 2.0f), p1.y + ((p2.y - p1.y) / 2.0f), 0));
			go->SetColour(glm::vec3(137.0f / 255.0f));

			pWorld.AddObject(go, true);
			res.push_back(go);
		}
		else if (type == "box")
		{
			glm::vec2 p1, p2;
			ss >> p1.x >> p1.y >> p2.x >> p2.y;

			GameObject* go = new GameObject();
			go->AddShape(new Quad(), glm::vec3(0), 0, glm::vec2(p2.x - p1.x, p1.y - p2.y));
			go->SetPosition(glm::vec3(p1.x + ((p2.x - p1.x) / 2.0f), p1.y + ((p2.y - p1.y) / 2.0f), 0));
			go->SetColour(glm::vec3(1.0f, 218.0f/255.0f, 0.0f));

			pWorld.AddObject(go);
			res.push_back(go);
		}
	}

	file.close();

	return res;
}