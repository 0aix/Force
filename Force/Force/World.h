#pragma once

#include <Box2D.h>
#include "Object.h"

class World
{
public:
	World();

private:
	b2World* world;
	//TerrainFactory* factory;
};