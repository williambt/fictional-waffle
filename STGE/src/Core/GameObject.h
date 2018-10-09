#pragma once
#include "../Rendering/Shapes/Shape.h"
#include <vector>
#include "../Physics/PhysicsBody.h"

class GameObject
{
	friend class PhysicsWorld;
private:
	glm::vec3 _position;
	glm::vec2 _scale;
	float _rotation;
	glm::vec3 _colour;
	Shader* _shader;

	std::vector<Shape*> _shapes;

	struct ShapeData
	{
		glm::vec3 position;
		float rotation;
		glm::vec2 scale;
		std::string id;
		b2Fixture* fixture;
	};

	std::vector<ShapeData> _shapeData;

	PhysicsBody* _physicsBody;

public:

	GameObject() : _shader(Shader::GetBasic()), _colour(glm::vec3(1)), _rotation(0), _scale(glm::vec3(1)), _position(glm::vec3(0)), _physicsBody(nullptr) {}
	~GameObject() { if (_physicsBody != nullptr) delete _physicsBody; }

	void Draw()
	{
		_shader->Uniform3f("colour", _colour);

		for (int i = 0; i < _shapes.size(); ++i)
		{
							//Object Transform
			glm::mat4 m = glm::translate(glm::mat4(1), _position)
				* glm::rotate(glm::mat4(1), glm::radians(_rotation), glm::vec3(0, 0, 1))
				//Shape Transform
				* glm::translate(glm::mat4(1), _shapeData[i].position)
				* glm::rotate(glm::mat4(1), glm::radians(_shapeData[i].rotation), glm::vec3(0, 0, 1))
				//Scale
				* glm::scale(glm::mat4(1), glm::vec3(_shapeData[i].scale, 1.0f))
				* glm::scale(glm::mat4(1), glm::vec3(_scale, 1.0f));

			_shader->UniformMat4f("modelMat", m);
			_shapes[i]->Draw();
		}
	}

	glm::vec3 GetPosition() const { return _position; }
	void SetPosition(const glm::vec3& pos) { _position = pos; }
	void Move(const glm::vec3& pos) { _position += pos; }
	void SetPosition(const glm::vec2& pos) { _position = glm::vec3(pos, _position.z); }
	void Move(const glm::vec2& pos) { _position += glm::vec3(pos, 0.0f); }

	glm::vec3 GetColour() const { return _colour; }
	void SetColour(const glm::vec3& colour) { _colour = colour; }

	glm::vec2 GetScale() const { return _scale; }
	void SetScale(const glm::vec2& scale) { _scale = scale; }
	void Scale(const glm::vec2& scale) { _scale += scale; }

	float GetRotation() const { return _rotation; }
	void SetRotation(const float rotation) { _rotation = rotation; }
	void Rotate(const float angle) { _rotation += angle; }

	Shader* GetShader() { return _shader; }
	void SetShader(Shader* shader) { _shader = shader; }

	void AddShape(Shape* shape, glm::vec3 position = glm::vec3(0), float rotation = 0, glm::vec2 scale = glm::vec2(1), std::string id = "")
	{
		_shapes.push_back(shape);
		_shapeData.push_back({ position, rotation, scale, id, nullptr });
	}

	b2Fixture* GetFixture(std::string id)
	{
		for (ShapeData& sd : _shapeData)
		{
			if (sd.id == id)
			{
				return sd.fixture;
			}
		}
		return nullptr;
	}

	PhysicsBody* GetPhysicsBody() { return _physicsBody; }
};