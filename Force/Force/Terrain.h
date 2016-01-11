#pragma once

#include "Object.h"

class Terrain : public Object
{
public:
	Terrain(b2Body* b) : Object(b) { type = ObjectType::TERRAIN; }
};