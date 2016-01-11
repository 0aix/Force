#include "stdafx.h"
#include "Object.h"

Player::Player(b2Body* b) : Entity(b) 
{
	type = ObjectType::PLAYER;
	hitbox = NULL;
	anim_list = NULL;
	anim_count = 0;
}

void Player::Update()
{
	b2Vec2 vel = body->GetLinearVelocity();
	float v = vel.x;
	if (move_state == -1)
		v = v < -4.0f ? v : -4.0f;
	else if (move_state == 1)
		v = v > 4.0f ? v : 4.0f;
	float dv = v - vel.x;
	if (dv > 0.3f)
		dv = 0.3f;
	else if (dv < -0.3f)
		dv = -0.3f;
	body->ApplyLinearImpulse(b2Vec2(body->GetMass() * dv, 0.0f), body->GetWorldCenter(), true);

	in_air = true;
	b2ContactEdge* edge = body->GetContactList();
	b2WorldManifold manifold;
	while (edge != NULL)
	{
		edge->contact->GetWorldManifold(&manifold);
		if (manifold.points[0].y > body->GetPosition().y)
		{
			in_air = false;
			break;
		}
		edge = edge->next;
	}

	if (jump && !in_air && jumpcd == 0)
	{
		dv = -4.0f - vel.y;
		body->ApplyLinearImpulse(b2Vec2(0.0f, body->GetMass() * dv), body->GetWorldCenter(), true);
		//body->SetLinearVelocity(b2Vec2(vel.x, -4.0f)
		jumpcd = 15;
	}
	if (jumpcd > 0)
		--jumpcd;
	if (fpush && fpushcd == 0)
	{
		FPush();
		fpushcd = 30;
	}
	if (fpushcd > 0)
		--fpushcd;
	if (push != NULL)
		push->Update();
	if (fpull && fpullcd == 0)
	{
		FPull();
		fpullcd = 30;
	}
	if (fpullcd > 0)
		--fpullcd;
	if (pull != NULL)
		pull->Update();
}

void Player::FPush()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	b2Vec2 pos = body->GetPosition();
	bodyDef.position.Set(pos.x, pos.y);
	b2Body* circle = body->GetWorld()->CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0.0f, 0.0f);
	circleShape.m_radius = 0.21875f;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	//circleDef.isSensor = true;
	circleDef.density = 0.00001f;
	circleDef.filter.categoryBits = 4;
	circleDef.filter.maskBits = 4;
	circleDef.userData = (void*)2;
	circle->CreateFixture(&circleDef);

	b2DistanceJointDef jointDef;
	jointDef.Initialize(body, circle, pos, pos);
	jointDef.collideConnected = false;
	jointDef.frequencyHz = 4.0f;
	jointDef.dampingRatio = 1.0f;
	body->GetWorld()->CreateJoint(&jointDef);
	

	push = new Push(circle);
	circle->SetUserData(this);
}

void Player::FPull()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	b2Vec2 pos = body->GetPosition();
	bodyDef.position.Set(pos.x, pos.y);
	b2Body* circle = body->GetWorld()->CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0.0f, 0.0f);
	circleShape.m_radius = 1.09375f;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	//circleDef.isSensor = true;
	circleDef.density = 0.00001f;
	circleDef.filter.categoryBits = 4;
	circleDef.filter.maskBits = 4;
	circleDef.userData = (void*)3;
	circle->CreateFixture(&circleDef);

	b2DistanceJointDef jointDef;
	jointDef.Initialize(body, circle, pos, pos);
	jointDef.collideConnected = false;
	jointDef.frequencyHz = 4.0f;
	jointDef.dampingRatio = 1.0f;
	body->GetWorld()->CreateJoint(&jointDef);


	pull = new Pull(circle);
	circle->SetUserData(this);
}