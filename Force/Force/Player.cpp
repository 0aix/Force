#include "stdafx.h"
#include "Object.h"

Player::Player(b2Body* b) : Entity(b) 
{
	type = ObjectType::PLAYER;
	hitbox = NULL;
	anim_list = NULL;
	anim_count = 0;
}

Player::~Player()
{
	delete push;
	delete pull;
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
	
	if (fpickup)
		FPickUp();
	if (fpickupcd > 0)
	{
		--fpickupcd;
		if (fpickupcd == 60)
		{
			b2Filter filter = {};
			filter.categoryBits = 4;
			filter.maskBits = 5;
			ball->GetFixtureList()->SetFilterData(filter);
		}
		else if (fpickupcd == 0)
		{
			fpower.x = 0.0f;
			fpower.y = 280.0f;
			FPass();
		}
	}
	if (!hasball && fpush && fpushcd == 0)
	{
		FPush();
		fpushcd = 30;
	}
	if (fpushcd > 0)
		--fpushcd;
	if (push != NULL)
		push->Update();
	if (!hasball && fpull && fpullcd == 0)
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
	jointDef.frequencyHz = 0.0f;
	jointDef.dampingRatio = 1.0f;
	body->GetWorld()->CreateJoint(&jointDef);
	
	circle->SetUserData(this);
	push = new Push(circle);
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
	jointDef.frequencyHz = 0.0f;
	jointDef.dampingRatio = 1.0f;
	body->GetWorld()->CreateJoint(&jointDef);

	circle->SetUserData(this);
	pull = new Pull(circle);
}

void Player::FPickUp()
{
	b2DistanceJointDef jointDef;
	b2Vec2 pos = body->GetPosition();
	ball->SetTransform(pos, ball->GetAngle());
	ball->SetAngularVelocity(0.0f);
	b2MassData data;
	ball->GetMassData(&data);
	data.mass = 0.00001f;
	ball->SetMassData(&data);
	b2Filter filter = {};
	jointDef.Initialize(body, ball, pos, pos);
	jointDef.collideConnected = false;
	jointDef.frequencyHz = 0.0f;
	jointDef.dampingRatio = 1.0f;
	balljoint = body->GetWorld()->CreateJoint(&jointDef);
	hasball = true;
	fpickup = false;
	fpickupcd = 120;
}

void Player::FReady()
{
	fpass = true;
	fpoint.SetZero();
	fpower.SetZero();

	//create the power vector
	b2BodyDef vecDef;
	vecDef.type = b2_dynamicBody;
	vecDef.fixedRotation = true;

	b2Vec2 pos = body->GetPosition();
	vecDef.position = pos;
	powervector = body->GetWorld()->CreateBody(&vecDef);

	b2EdgeShape edge;
	edge.Set(b2Vec2_zero, b2Vec2_zero);

	b2FixtureDef edgeDef;
	edgeDef.shape = &edge;
	edgeDef.density = 0.00001f;
	edgeDef.filter.categoryBits = 0;
	edgeDef.filter.maskBits = 0;
	powervector->CreateFixture(&edgeDef);
	b2MassData mass;
	powervector->GetMassData(&mass);
	mass.mass = 0.00001f;
	powervector->SetMassData(&mass);

	b2DistanceJointDef jointDef;
	jointDef.Initialize(body, powervector, pos, pos);
	jointDef.collideConnected = false;
	jointDef.frequencyHz = 0.0f;
	jointDef.dampingRatio = 1.0f;
	body->GetWorld()->CreateJoint(&jointDef);
}

void Player::FCharge(float x, float y)
{
	fpoint.x += 1.5 * x;
	fpoint.y += 1.5 * y;
	fpower = fpoint;
	
	if (fpoint.LengthSquared() > 900.0f * 900.0f)
	{
		fpower.Normalize();
		fpower *= 900.0f;
	}

	powervector->DestroyFixture(powervector->GetFixtureList());

	//create the power vector fixture
	b2EdgeShape edge;
	edge.Set(b2Vec2_zero, 0.003f * fpower);

	b2FixtureDef edgeDef;
	edgeDef.shape = &edge;
	edgeDef.density = 0.00001f;
	edgeDef.filter.categoryBits = 0;
	edgeDef.filter.maskBits = 0;
	powervector->CreateFixture(&edgeDef);
	b2MassData mass;
	powervector->GetMassData(&mass);
	mass.mass = 0.00001f;
	powervector->SetMassData(&mass);

	b2DistanceJointDef jointDef;
	b2Vec2 pos = body->GetPosition();
	jointDef.Initialize(body, powervector, pos, pos);
	jointDef.collideConnected = false;
	jointDef.frequencyHz = 0.0f;
	jointDef.dampingRatio = 1.0f;
	body->GetWorld()->CreateJoint(&jointDef);
}

void Player::FPass()
{
	body->GetWorld()->DestroyJoint(balljoint);
	ball->ResetMassData();
	b2Filter filter = {};
	filter.categoryBits = 4;
	filter.maskBits = 5;
	ball->GetFixtureList()->SetFilterData(filter);
	b2Vec2 impulse = -0.001f * fpower;
	//ball->SetLinearVelocity(b2Vec2_zero);
	ball->SetLinearVelocity(body->GetLinearVelocity());
	ball->SetAngularVelocity(0.0f);
	ball->ApplyLinearImpulse(impulse, ball->GetWorldCenter(), true);
	fpass = false;
	fpickupcd = 0;
	hasball = false;

	//destroyyyyyyyy
	if (powervector)
	{
		body->GetWorld()->DestroyBody(powervector);
		powervector = NULL;
	}
}