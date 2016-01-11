#include "stdafx.h"
#include "Object.h"

Push::Push(b2Body* b) : Entity(b)
{
	type = ObjectType::PUSH;
	hitbox = NULL;
	anim_list = NULL;
	anim_count = 0;
	num = 0;
	radius = 0.21875f;
	player = (Player*)body->GetUserData();
}

void Push::Update()
{
	const int ticks = 10;
	const float inc = 0.875f / ticks;
	if (num < ticks)
	{
		radius += inc;
		body->DestroyFixture(body->GetFixtureList());

		b2CircleShape circleShape;
		circleShape.m_p.Set(0.0f, 0.0f);
		circleShape.m_radius = radius;

		b2FixtureDef circleDef;
		circleDef.shape = &circleShape;
		//circleDef.isSensor = true;
		circleDef.density = 0.00001f;
		circleDef.filter.categoryBits = 4;
		circleDef.filter.maskBits = 4;
		circleDef.userData = (void*)2;
		body->CreateFixture(&circleDef);

		num++;
	}
	else
	{
		Player* player = (Player*)body->GetUserData();
		player->push = NULL;
		delete this;
	}
}

Pull::Pull(b2Body* b) : Entity(b)
{
	type = ObjectType::PULL;
	hitbox = NULL;
	anim_list = NULL;
	anim_count = 0;
	num = 0;
	radius = 1.09375f;
	player = (Player*)body->GetUserData();
}

void Pull::Update()
{
	const int ticks = 10;
	const float inc = 0.875f / ticks;
	if (num < ticks)
	{
		radius -= inc;
		body->DestroyFixture(body->GetFixtureList());

		b2CircleShape circleShape;
		circleShape.m_p.Set(0.0f, 0.0f);
		circleShape.m_radius = radius;

		b2FixtureDef circleDef;
		circleDef.shape = &circleShape;
		//circleDef.isSensor = true;
		circleDef.density = 0.00001f;
		circleDef.filter.categoryBits = 4;
		circleDef.filter.maskBits = 4;
		circleDef.userData = (void*)3;
		body->CreateFixture(&circleDef);

		num++;
	}
	else
	{
		Player* player = (Player*)body->GetUserData();
		player->pull = NULL;
		delete this;
	}
}