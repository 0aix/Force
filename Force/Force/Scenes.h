#pragma once

//PRECOMPILED
#include "Scene.h"
//***********

#include "Draw.h"
#include "World.h"

class MainScene : public Scene
{
public:
	MainScene();
	void Release();
	void Update();
	void HandleInput();
	void Render();
};

class LoadScene : public Scene
{
public:
	LoadScene();
	void Release();
	void Update();
	void HandleInput();
	void Render();
};

class EndScene : public Scene
{
public:
	EndScene();
	void Release();
	void Update();
	void HandleInput();
	void Render();
};

class TestScene : public Scene
{
public:
	TestScene();
	void Release();
	void Update();
	void HandleInput();
	void Render();

private:
	b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
	b2World world = b2World(gravity);

	Camera camera;
	Clock clock;
	double dTime = 0.0;

	DebugDraw debugdraw;
};