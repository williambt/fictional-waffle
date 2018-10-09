#include "PhysicsBody.h"
#include "PhysicsWorld.h"

PhysicsBody::PhysicsBody(b2Body * body) : _body(body)
{
	_mass = body->GetMass();
	_lockRotation = body->IsFixedRotation();
	_friction = body->GetFixtureList()->GetFriction();
}

PhysicsBody::PhysicsBody(b2Body * body, PhysicsWorld * world) : _body(body), _world(world)
{
	_mass = body->GetMass();
	_lockRotation = body->IsFixedRotation();
	_friction = body->GetFixtureList()->GetFriction();
}

PhysicsBody::~PhysicsBody()
{
	if (_body != nullptr)
	{
		_world->DestroyBody(this);
	}
}

void PhysicsBody::LockRotation(bool lock)
{
	_lockRotation = lock;
	_body->SetFixedRotation(lock);
}

void PhysicsBody::ApplyForce(glm::vec2 force, glm::vec2 point)
{
	_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

void PhysicsBody::ApplyForceToCenter(glm::vec2 force)
{
	_body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}