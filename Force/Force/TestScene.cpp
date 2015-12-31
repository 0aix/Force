#include "stdafx.h"
#include "Scenes.h"
#include "World.h"

TestScene::TestScene() : Scene()
{
	world.SetDebugDraw(&debugdraw);
	debugdraw.SetFlags(DebugDraw::e_shapeBit);

	b2BodyDef groundDef;
	groundDef.type = b2_staticBody;
	groundDef.position.Set(12.5f, 11.71875f);
	b2Body* body = world.CreateBody(&groundDef);

	b2PolygonShape rectShape;
	rectShape.SetAsBox(12.5f, 0.78125f);

	b2FixtureDef rectDef;
	rectDef.shape = &rectShape;
	rectDef.friction = 0.3f;
	rectDef.restitution = 0.3f;
	rectDef.density = 1.0f;
	body->CreateFixture(&rectDef);

	groundDef.position.Set(0.3125f, 6.25f);
	body = world.CreateBody(&groundDef);
	rectShape.SetAsBox(0.3125f, 6.25f);
	body->CreateFixture(&rectDef);

	groundDef.position.Set(24.6875f, 6.25f);
	body = world.CreateBody(&groundDef);
	body->CreateFixture(&rectDef);

	groundDef.position.Set(12.5f, 0.625f);
	body = world.CreateBody(&groundDef);
	rectShape.SetAsBox(12.5f, 0.625f);
	body->CreateFixture(&rectDef);

	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.position.Set(12.5f, 6.25f);
	body = world.CreateBody(&ballDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0.0f, 0.0f);
	circleShape.m_radius = 0.3125f;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.friction = 0.3f;
	circleDef.restitution = 0.7f;
	circleDef.density = 1.0f;
	body->CreateFixture(&circleDef);

	camera.x = 0.0f;
	camera.y = -3.125f;
	camera.scale = 32.0f;

	clock.Start();
}

void TestScene::Release()
{
	delete this;
}

void TestScene::Update()
{
	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;
	dTime += clock.Time();
	while (dTime >= C::TICK)
	{

		world.Step(C::TICK, velocityIterations, positionIterations);
		dTime -= C::TICK;
		//if (dTime < C::TICK)
		//	Render();
	}
}
void TestScene::HandleInput() {}
void TestScene::Render()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	d3ddev->BeginScene();

	debugdraw.Draw(world, camera);

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}