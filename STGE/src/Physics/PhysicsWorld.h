#pragma once

#include "../Core/GameObject.h"
#include <Box2D/Box2D.h>
#include <glm/vec2.hpp>
#include <vector>
#include "../Rendering/Shapes/Shape.h"
#include <iostream>

static b2Vec2 glmTob2(const glm::vec2& vec)
{
	return b2Vec2(vec.x, vec.y);
}

static glm::vec2 b2Toglm(const b2Vec2& vec)
{
	return glm::vec2(vec.x, vec.y);
}

class PhysicsWorld
{
private:
	b2World* _world;

	int velocityIterations;
	int positionIterations;
	float timeStep;

public:

	PhysicsWorld(glm::vec2 gravity = glm::vec2(0, -9.8f))
	{
		_world = new b2World(glmTob2(gravity));
		velocityIterations = 10;
		positionIterations = 8;
		timeStep = 1.0f / 60.0f;
	}

	void AddObject(GameObject* object, bool isStatic = false, uint16 category = 0, uint16 mask = 0xffff)
	{
		b2BodyDef bodyDef;
		bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody;
		bodyDef.position = glmTob2(object->GetPosition());
		bodyDef.angle = glm::radians(object->GetRotation());
		b2Body* body = _world->CreateBody(&bodyDef);
		body->SetUserData(object);

		const std::vector<Shape*>& shapes = object->_shapes;
		std::vector<GameObject::ShapeData>& shapeData = object->_shapeData;

		for (int i = 0; i < shapes.size(); ++i)
		{
			b2Shape* bShape = nullptr;
			if (strcmp(shapes[i]->ShapeName(), "Quad") == 0)
			{
				bShape = new b2PolygonShape();
				b2PolygonShape* polyShape = (b2PolygonShape*)bShape;
				polyShape->SetAsBox(shapeData[i].scale.x / 2.0f, shapeData[i].scale.y / 2.0f, glmTob2(shapeData[i].position), glm::radians(shapeData[i].rotation));
			}
			else if (strcmp(shapes[i]->ShapeName(), "Circle") == 0)
			{
				bShape = new b2CircleShape();
				b2CircleShape* circleShape = (b2CircleShape*)bShape;
				circleShape->m_radius = shapeData[i].scale.x;
				circleShape->m_p = glmTob2(shapeData[i].position);
			}
			b2FixtureDef fixtureDef;
			fixtureDef.shape = bShape;
			fixtureDef.density = 1.0f;
			if (category > 0)
			{
				fixtureDef.filter.categoryBits = category;
				fixtureDef.filter.maskBits = mask;
			}
			b2Fixture* fixture = body->CreateFixture(&fixtureDef);
			shapeData[i].fixture = fixture;
		}

		object->_physicsBody = new PhysicsBody(body, this);
	}

	void AddEdge(glm::vec2 p1, glm::vec2 p2)
	{
		b2BodyDef bodyDef;
		b2Body* body = _world->CreateBody(&bodyDef);

		b2EdgeShape shape;
		shape.Set(glmTob2(p1), glmTob2(p2));
		body->CreateFixture(&shape, 1.0f);
	}

	void Update(double timeStep)
	{
		_world->Step(timeStep, velocityIterations, positionIterations);
		_world->ClearForces();

		b2Body* bod = _world->GetBodyList();

		for (int i = 0; i < _world->GetBodyCount(); ++i)
		{
			GameObject* object = (GameObject*)bod->GetUserData();
			if (object)
			{
				object->SetPosition(b2Toglm(bod->GetPosition()));
				object->SetRotation(glm::degrees(bod->GetAngle()));
			}
			bod = bod->GetNext();
		}
	}

	void CreateRevoluteJoint(GameObject* obj1, GameObject* obj2, glm::vec2 anchor1 = glm::vec2(0), glm::vec2 anchor2 = glm::vec2(0), bool collideConnected = false)
	{
		b2Body* b1 = obj1->GetPhysicsBody()->GetB2Body();
		b2Body* b2 = obj2->GetPhysicsBody()->GetB2Body();

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = b1;
		jointDef.bodyB = b2;
		jointDef.localAnchorA = glmTob2(anchor1);
		jointDef.localAnchorB = glmTob2(anchor2);
		jointDef.collideConnected = collideConnected;

		b2RevoluteJoint* joint = (b2RevoluteJoint*)_world->CreateJoint(&jointDef);
		obj1->GetPhysicsBody()->_joints.push_back(joint);
		obj2->GetPhysicsBody()->_joints.push_back(joint);
	}

	void DestroyJoint(b2Joint* joint)
	{
		std::list<b2Joint*>& jointsA = ((GameObject*)joint->GetBodyA()->GetUserData())->GetPhysicsBody()->_joints;
		std::list<b2Joint*>& jointsB = ((GameObject*)joint->GetBodyB()->GetUserData())->GetPhysicsBody()->_joints;
		jointsA.remove(joint);
		jointsB.remove(joint);

		_world->DestroyJoint(joint);
	}

	void DestroyBody(PhysicsBody* body)
	{
		for (b2Joint* j : body->_joints)
		{
			PhysicsBody* other = body->GetB2Body() == j->GetBodyA() 
				? ((GameObject*)j->GetBodyB()->GetUserData())->_physicsBody 
				: ((GameObject*)j->GetBodyA()->GetUserData())->_physicsBody;
			other->_joints.remove(j);
			_world->DestroyJoint(j);
		}
		_world->DestroyBody(body->GetB2Body());
	}

	b2World* GetB2World() { return _world; }
};