#pragma once

#include <Box2D.h>
#include "Object.h"
#include "Terrain.h"
#include "Draw.h"
#include "ContactListener.h"

class World
{
public:
	World();
	~World();
	void Update();
	Player* CreatePlayer(float x, float y);
	Object* CreateBall(float x, float y, float r);
	Terrain* CreateRectGround(float x, float y, float l, float h, float f = 0.3f, float r = 0.2f, float d = 1.0f);
	Terrain* CreatePolyGround(float x, float y, b2Vec2* vertices, int count, float f = 0.3f, float r = 0.2f, float d = 1.0f);

	b2World* GetWorld() const { return world; }

private:
	b2World* world;
	Player* player;
	Object* ball;
	vector<Object*> object_list;
	ContactListener contactListener;
	//float scale; //change everything by scale for ezpzness
	//debugdraw dunno maybe
};