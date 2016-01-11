#include "stdafx.h"
#include "World.h"

World::World()
{
	b2Vec2 gravity(0.0f, 10.0f);
	world = new b2World(gravity);
	world->SetContactListener(&contactListener);
}

World::~World()
{
	vector<Object*>::iterator end = object_list.end();
	for (vector<Object*>::iterator it = object_list.begin(); it != end; ++it)
		delete *it;
	delete world;
}

void World::Update()
{
	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;

	player->Update();
	world->Step(C::TICK, velocityIterations, positionIterations);
}

Player* World::CreatePlayer(float x, float y)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	b2Body* body = world->CreateBody(&bodyDef);
	body->SetFixedRotation(true);

	b2PolygonShape rectShape;
	rectShape.SetAsBox(0.3125f, 0.46875f);

	b2FixtureDef rectDef;
	rectDef.shape = &rectShape;
	rectDef.friction = 0.3f;
	rectDef.restitution = 0.2f;
	rectDef.density = 1.0f;
	rectDef.filter.categoryBits = 2;
	rectDef.filter.maskBits = 1;
	//body->CreateFixture(&rectDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0.0f, 0.0f);
	circleShape.m_radius = 0.21875f;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.friction = 0.3f;
	circleDef.restitution = 0.2f;
	circleDef.density = 1.0f;
	//circleDef.filter.categoryBits = 4;
	//circleDef.filter.maskBits = 4;
	circleDef.filter.categoryBits = 2;
	circleDef.filter.maskBits = 1;
	body->CreateFixture(&circleDef);

	player = new Player(body);
	body->SetUserData(player);
	object_list.push_back(player);

	//set userdata for hitbox fixture
	//let constructor set userdata for body. ez

	return player;
}

Object* World::CreateBall(float x, float y, float r)
{
	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.position.Set(x, y);
	b2Body* body = world->CreateBody(&ballDef);
	body->SetLinearDamping(0.5f);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0.0f, 0.0f);
	circleShape.m_radius = r;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.friction = 0.3f;
	circleDef.restitution = 0.7f;
	circleDef.density = 1.0f;
	circleDef.filter.categoryBits = 4;
	circleDef.filter.maskBits = 5;
	circleDef.userData = (void*)1;
	body->CreateFixture(&circleDef);

	Object* ball = new Object(body);
	body->SetUserData(ball);
	object_list.push_back(ball);

	return ball;
}

Terrain* World::CreateRectGround(float x, float y, float l, float h, float f, float r, float d)
{
	b2BodyDef groundDef;
	groundDef.type = b2_staticBody;
	groundDef.position.Set(x, y);
	b2Body* body = world->CreateBody(&groundDef);

	b2PolygonShape rectShape;
	rectShape.SetAsBox(l / 2.0f, h / 2.0f);

	b2FixtureDef rectDef;
	rectDef.shape = &rectShape;
	rectDef.friction = f;
	rectDef.restitution = r;
	rectDef.density = d;
	body->CreateFixture(&rectDef);

	Terrain* t = new Terrain(body);
	body->SetUserData(t);
	object_list.push_back(t);

	return t;
}

Terrain* World::CreatePolyGround(float x, float y, b2Vec2* vertices, int count, float f, float r, float d)
{
	b2BodyDef groundDef;
	groundDef.type = b2_staticBody;
	groundDef.position.Set(x, y);
	b2Body* body = world->CreateBody(&groundDef);

	b2PolygonShape polyShape;
	polyShape.Set(vertices, count);

	b2FixtureDef polyDef;
	polyDef.shape = &polyShape;
	polyDef.friction = f;
	polyDef.restitution = r;
	polyDef.density = d;
	body->CreateFixture(&polyDef);

	Terrain* t = new Terrain(body);
	body->SetUserData(t);
	object_list.push_back(t);

	return t;
}