#pragma once
#include "Scene.h"
#include "../Rendering/Shapes/Circle.h"
#include "../Rendering/Shapes/Quad.h"

#include "../MapReader.h"

class MainScene : public Scene
{
private:

	GameObject * _player;

	float remap(float val, float min, float max, float nmin, float nmax)
	{
		return (val - min) / (max - min) * (nmax - nmin) + nmin;
	}

	bool _shooting = false;

	GameObject* _grapple = nullptr;
	std::vector<GameObject*> _grappleChain;

	int _fps = 0;
	float _secondTimer = 0;

	enum _cllisionCategory
	{
		WORLD = 0x0001,
		PLAYER = 0x0002
	};

	float _jumpForce = 5000;
	float _moveForce = 300;
	bool _canJump = false;

	
	float _chainLength = 0.5f;
	void CreateChain()
	{
		glm::vec2 ptog = _grapple->GetPosition() - _player->GetPosition();
		int nOfChains = glm::length(ptog) / _chainLength;

		ptog = ptog / (float)nOfChains;
		for (int i = 0; i < nOfChains; ++i)
		{
			GameObject* c = new GameObject();
			c->AddShape(new Quad(), glm::vec3(0), 0, glm::vec2(_chainLength, 0.139f));
			c->SetColour(glm::vec3(0.38f, 0.61f, 0.0f));
			c->SetPosition(glm::vec2(_player->GetPosition()) + (ptog * (float)i));
			_world.AddObject(c, false, WORLD, WORLD);

			c->GetPhysicsBody()->SetMass(-1);
			//c->GetPhysicsBody()->GetB2Body()->SetGravityScale(0.1f);


			if (i == 0)
			{
				_world.CreateRevoluteJoint(_player, c, glm::vec2(0), glm::vec2(-0.25f, 0.0f));
			}
			else
			{
				_world.CreateRevoluteJoint(c, _grappleChain.back(), glm::vec2(-0.25f, 0.0f), glm::vec2(0.25f, 0.0f));
			}
			if (i == nOfChains - 1)
			{
				_world.CreateRevoluteJoint(_grapple, c, glm::vec2(0), glm::vec2(0.25f, 0.0f));
			}

			_grappleChain.push_back(c);
			_gameObjects.push_back(c);
		}
	}
	void DestroyChain()
	{
		for (GameObject* c : _grappleChain)
		{
			RemoveGameObject(c);
			delete c;
		}

		_grappleChain.clear();
	}

public:

	MainScene(Window& window) : Scene(window) {}
	~MainScene() {}

	virtual void Setup()
	{
		_gameObjects = readMLO("res/test2.mlo", _world);
		
		_gameObjects.push_back(new GameObject());

		_player = _gameObjects.back();
		_player->AddShape(new Quad(), glm::vec3(0), 0, glm::vec3(1, 2, 1)/2.0f);
		_player->AddShape(new Circle(), glm::vec3(0, 1, 0)/2.0f, 0, glm::vec3(0.25f));
		_player->AddShape(new Circle(), glm::vec3(0, -1, 0)/2.0f, 0, glm::vec3(0.25f), "feet");
		_world.AddObject(_player, false , PLAYER);

		_player->GetPhysicsBody()->LockRotation(true);
		_player->GetPhysicsBody()->SetMass(10);
	}

	virtual void Update()
	{
		//QUIT on escape
		if (Input::GetKeyReleased(GLFW_KEY_ESCAPE))
		{
			window.Close();
		}

		//MOVEMENT
		if (Input::GetKeyDown(GLFW_KEY_A)) //Left
		{
			_player->GetPhysicsBody()->ApplyForceToCenter(glm::vec2(-_moveForce, 0));
			b2Vec2 xVel = _player->GetPhysicsBody()->GetB2Body()->GetLinearVelocity();
			if (xVel.x < -10)
			{
				xVel.x = -10;
				_player->GetPhysicsBody()->GetB2Body()->SetLinearVelocity(xVel);
			}
		}
		if (Input::GetKeyDown(GLFW_KEY_D)) //Right
		{
			_player->GetPhysicsBody()->ApplyForceToCenter(glm::vec2(_moveForce, 0));
			b2Vec2 xVel = _player->GetPhysicsBody()->GetB2Body()->GetLinearVelocity();
			if (xVel.x > 10)
			{
				xVel.x = 10;
				_player->GetPhysicsBody()->GetB2Body()->SetLinearVelocity(xVel);
			}
		}
		if (Input::GetKeyReleased(GLFW_KEY_A) || Input::GetKeyReleased(GLFW_KEY_D)) //Stop
		{
			b2Vec2 xVel = _player->GetPhysicsBody()->GetB2Body()->GetLinearVelocity();
			xVel.x = 0;
			_player->GetPhysicsBody()->GetB2Body()->SetLinearVelocity(xVel);
		}
		if (_canJump && Input::GetKeyPressed(GLFW_KEY_W)) //Jump
		{
			_player->GetPhysicsBody()->ApplyForceToCenter(glm::vec2(0, _jumpForce));
		}

		//Check if player can jump
		b2ContactEdge* pContacts = _player->GetPhysicsBody()->GetB2Body()->GetContactList();
		b2Fixture* pFixture = _player->GetFixture("feet");
		bool grounded = false;
		while (pContacts)
		{
			if (pContacts->contact->GetFixtureA() == pFixture || pContacts->contact->GetFixtureB() == pFixture)
			{
				b2CircleShape* circle = (b2CircleShape*)pFixture->GetShape();
				b2WorldManifold manifold;
				pContacts->contact->GetWorldManifold(&manifold);
				if (manifold.points[0].y < (circle->m_p.y + _player->GetPosition().y - (circle->m_radius / 2.0f))
					&& manifold.points[0].x < (circle->m_p.x + _player->GetPosition().x + (circle->m_radius / 2.0f))
					&& manifold.points[0].x > (circle->m_p.x + _player->GetPosition().x - (circle->m_radius / 2.0f)))
				{
					_canJump = true;
					grounded = true;
				}
			}
			pContacts = pContacts->next;
		}
		if (!grounded)
			_canJump = false;

		if (Input::GetMButtonPressed(GLFW_MOUSE_BUTTON_1) && !_grapple)
		{
			_grapple = new GameObject();
			_grapple->AddShape(new Circle(), glm::vec3(0), 0, glm::vec2(0.25f));
			_grapple->SetPosition(_player->GetPosition());
			_grapple->SetColour(glm::vec3(0.4f));
			_world.AddObject(_grapple, false, WORLD, WORLD);
			_gameObjects.push_back(_grapple);

			glm::vec2 ptom = window.ScreenToWorld(Input::GetMousePos()) - glm::vec2(_player->GetPosition());
			ptom = glm::normalize(ptom);
			_grapple->GetPhysicsBody()->GetB2Body()->SetGravityScale(0);
			_grapple->GetPhysicsBody()->GetB2Body()->SetLinearVelocity(glmTob2(ptom * 25.0f));
			_shooting = true;
		}
		else if (Input::GetMButtonPressed(GLFW_MOUSE_BUTTON_1) && _grapple)
		{
			DestroyChain();
			RemoveGameObject(_grapple);
			delete _grapple;
			_grapple = nullptr;
			_shooting = false;
		}

		b2ContactEdge* gContacts = nullptr;
		if(_grapple && !_grapple->GetPhysicsBody()->GetB2Body()->GetJointList()) gContacts = _grapple->GetPhysicsBody()->GetB2Body()->GetContactList();
		while (gContacts)
		{
			GameObject* other = (GameObject*)gContacts->other->GetUserData();

			b2WorldManifold manifold;
			manifold.points[0].Set(0, 0);
			gContacts->contact->GetWorldManifold(&manifold);
			if (manifold.points[0] != b2Vec2(0, 0))
			{
				manifold.points[0].Set(manifold.points[0].x - other->GetPosition().x, manifold.points[0].y - other->GetPosition().y);
				float angle = glm::radians(other->GetRotation());
				manifold.points[0].Set(cosf(angle) * manifold.points[0].x - sinf(angle) * manifold.points[0].y,
					sinf(angle) * manifold.points[0].x + cosf(angle) * manifold.points[0].y);
				_world.CreateRevoluteJoint(_grapple, other, glm::vec2(0), b2Toglm(manifold.points[0]) - glm::vec2(other->GetPosition()));
				CreateChain();
				_shooting = false;
				break;
			}
			gContacts = gContacts->next;
		}

		_frameTime = glfwGetTime() - _lastTime;
		_lastTime = glfwGetTime();

		_secondTimer += _frameTime;
		++_fps;
		if (_secondTimer >= 1.0f)
		{
			std::cout << "FPS: " << _fps << std::endl;
			_secondTimer -= 1.0f;
			_fps = 0;
		}

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

		if (_shooting && _grapple)
		{
			glLineWidth(5);
			Shader::GetBasic()->Uniform3f("colour", glm::vec3(0.38f, 0.61f, 0.0f));
			Shader::GetBasic()->UniformMat4f("modelMat", glm::mat4(1));
			glBegin(GL_LINES);
			{
				glVertex2f(_player->GetPosition().x, _player->GetPosition().y);
				glVertex2f(_grapple->GetPosition().x, _grapple->GetPosition().y);
			}
			glEnd();
		}

		window.SwapBuffers();

		return;

		//----------------Debug Box2D----------------------
		Shader::GetBasic()->UniformMat4f("modelMat", glm::mat4(1));
		Shader::GetBasic()->Uniform3f("colour", glm::vec3(1.0f, 0.0f, 0.0f));
		glLineWidth(5);

		b2Body* bod = _world.GetB2World()->GetBodyList();

		for (b2Body* b = _world.GetB2World()->GetBodyList(); b; b = b->GetNext())
		{
			const b2Transform& xf = b->GetTransform();
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				if (f->GetType() == b2Shape::e_circle)
				{
					Shader::GetBasic()->Uniform3f("colour", glm::vec3(1.0f, 0.0f, 1.0f));

					b2CircleShape* c = (b2CircleShape*)f->GetShape();
					glBegin(GL_LINES);
					{
						glVertex3f(b2Mul(xf, c->m_p).x, b2Mul(xf, c->m_p).y, -0.15f);
						glVertex3f(b2Mul(xf, c->m_p).x, b2Mul(xf, c->m_p).y - c->m_radius, -0.15f);
					}
					glEnd();
					Shader::GetBasic()->Uniform3f("colour", glm::vec3(1.0f, 0.0f, 0.0f));
				}
				else if (f->GetType() == b2Shape::e_edge)
				{
					b2EdgeShape* e = (b2EdgeShape*)f->GetShape();
					int vertexCount;

					const b2Transform& xf = b->GetTransform();

					b2Vec2 vertices[b2_maxPolygonVertices];
					int i = 0;
					if (e->m_hasVertex0)
					{
						vertices[i++] = b2Mul(xf, e->m_vertex0);
					}
					vertices[i++] = b2Mul(xf, e->m_vertex1);
					vertices[i++] = b2Mul(xf, e->m_vertex2);
					if (e->m_hasVertex3)
					{
						vertices[i++] = b2Mul(xf, e->m_vertex3);
					}

					vertexCount = i;

					glBegin(GL_POLYGON);
					{
						for (i = 0; i < vertexCount; ++i)
						{
							glVertex3f(vertices[i].x, vertices[i].y, -0.25f);
						}
					}
					glEnd();
				}
				else if (f->GetType() == b2Shape::e_polygon)
				{
					b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
					int32 vertexCount = poly->m_count;
					b2Assert(vertexCount <= b2_maxPolygonVertices);
					b2Vec2 vertices[b2_maxPolygonVertices];

					for (int32 i = 0; i < vertexCount; ++i)
					{
						vertices[i] = b2Mul(xf, poly->m_vertices[i]);
					}

					glBegin(GL_POLYGON);
					{
						for (int i = 0; i < vertexCount; ++i)
						{
							glVertex3f(vertices[i].x, vertices[i].y, -0.25f);
						}
					}
					glEnd();
				}
			}
		}

		window.SwapBuffers();
	}
};