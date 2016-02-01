#pragma once

//PRECOMPILED
#include <Box2D.h>
//***********
#include "Sprite.h"

//replace w/ bitmask
enum ObjectType
{
	OBJECT = -1,
	DYNAMIC = 0,
	TERRAIN = 1,
	ENTITY = 2,
	PLAYER = 3,
	MOB = 4,
	SKILL = 5,
	BALL = 6,
	PUSH = 7,
	PULL = 8
};

class Push;
class Pull;

//Object
class Object
{
public:
	Object(b2Body* b) { body = b; }
	virtual ~Object() { body->GetWorld()->DestroyBody(body); }

	ObjectType type = ObjectType::OBJECT;

protected:
	b2Body* body;
};

//DynamicObject interface
class DynamicObject : public Object
{
public:
	DynamicObject(b2Body* b) : Object(b) { type = ObjectType::DYNAMIC; }
	virtual void Update() = 0;

};

//Entity interface
class Entity : public DynamicObject
{
public:
	Entity(b2Body* b) : DynamicObject(b) { type = ObjectType::ENTITY; }
	//virtual ~Entity();

protected:
	b2Fixture* hitbox;
	Animation<b2FixtureDef> manager;
	Anim<b2FixtureDef>** anim_list;
	int anim_count;
};

//Player interface (yup, we're gonna polymorph the classes!)
class Player : public Entity
{
public:
	Player(b2Body* b);
	~Player();
	void Update();
	void FPush();
	void FPull();
	void FPickUp();
	void FReady();
	void FCharge(float x, float y);
	void FPass();

	int move_state = 0;
	bool jump = false;
	int jumpcd = 0;
	bool in_air = true;
	bool fpush = false;
	int fpushcd = 0;
	Push* push = NULL;
	bool fpull = false;
	int fpullcd = 0;
	Pull* pull = NULL;
	bool fpickup = false;
	int fpickupcd = 0;
	bool hasball = false;
	b2Body* ball;
	b2Joint* balljoint = NULL;
	bool fpass = false;
	b2Vec2 fpoint;
	b2Vec2 fpower;
	b2Body* powervector; //placeholder for an actual effect
};

//Mob interface
class Mob : public Entity
{
public:
	Mob(b2Body* b) : Entity(b) { type = ObjectType::MOB; }

};

class Push : public Entity
{
public:
	Push(b2Body* b);
	void Update();

	int num;
	float radius;
	Player* player;

};

class Pull : public Entity
{
public:
	Pull(b2Body* b);
	void Update();
	
	int num;
	float radius;
	Player* player;

};