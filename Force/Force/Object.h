#pragma once

//PRECOMPILED
#include <Box2D.h>
//***********
#include "Sprite.h"

enum ObjectType
{
	OBJECT = -1,
	DYNAMIC = 0,
	TERRAIN = 1,
	ENTITY = 2,
	PLAYER = 3,
	MOB = 4,
	SKILL = 5,
	OBSTACLE = 6,
};

//Object interface
class Object
{
public:
	Object(b2World* b2w) { world = b2w; }
	ObjectType type = ObjectType::OBJECT;

protected:
	b2World* world;
	b2Body* body;
	Animation<b2FixtureDef> manager;
	Anim<b2FixtureDef>** anim_list;
	int anim_count;
};

//DynamicObject interface
class DynamicObject : public Object
{
public:
	DynamicObject(b2World* b2w) : Object(b2w) { type = ObjectType::DYNAMIC; }
	virtual void Update() = 0;

protected:
};

//Entity interface
class Entity : public DynamicObject
{
public:
	Entity(b2World* b2w) : DynamicObject(b2w) { type = ObjectType::ENTITY; }

};

//Player interface (yup, we're gonna polymorph the classes!)
class Player : public Entity
{
public:
	Player(b2World* b2w) : Entity(b2w) { type = ObjectType::PLAYER; }
	void Update() {}

};

//Mob interface
class Mob : public Entity
{
public:
	Mob(b2World* b2w) : Entity(b2w) { type = ObjectType::MOB; }

};