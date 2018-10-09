#pragma once

#include "glm/vec2.hpp"
#include <Box2D/Box2D.h>
#include <list>

class PhysicsBody
{
	friend class PhysicsWorld;
private:
	float _mass;
	b2Body* _body;
	bool _lockRotation;

	float _friction;

	std::list<b2Joint*> _joints;

	PhysicsWorld* _world;


public:

	PhysicsBody() : _body(nullptr), _friction(0), _lockRotation(false), _mass(0) {}
	PhysicsBody(PhysicsWorld* world) : _world(world), _body(nullptr), _friction(0), _lockRotation(false), _mass(0) {}
	PhysicsBody(b2Body* body);
	PhysicsBody(b2Body* body, PhysicsWorld* world);
	~PhysicsBody();

	void LockRotation(bool lock);
	bool IsRotationLocked() const { return _lockRotation; }
	void ApplyForce(glm::vec2 force, glm::vec2 point);
	void ApplyForceToCenter(glm::vec2 force);

	void SetFriction(float friction)
	{
		_friction = friction;
		for (b2Fixture* f = _body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetFriction(friction);
		}
	}
	float GetFriction() const { return _friction; }

	void SetMass(float mass)
	{
		_mass = mass;
		b2MassData md;
		_body->GetMassData(&md);
		md.mass = mass;
		_body->SetMassData(&md);
	}
	float GetMass() const { return _mass; }

	b2Body* GetB2Body() { return _body; }
};