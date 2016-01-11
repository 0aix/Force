#include "stdafx.h"
#include "Scenes.h"
#include "World.h"

TestScene::TestScene() : Scene()
{
	world.GetWorld()->SetDebugDraw(&debugdraw);
	debugdraw.SetFlags(DebugDraw::e_shapeBit);

	world.CreateRectGround(12.5f, 11.71875f, 25.0f, 1.5625f);
	world.CreateRectGround(0.3125f, 6.25f, 0.625f, 12.5f);
	world.CreateRectGround(24.6875f, 6.25f, 0.625f, 12.5f);
	world.CreateRectGround(12.5f, 0.625f, 25.0f, 1.25f);

	b2Vec2 vertices[3];
	vertices[0].Set(0.0f, 0.0f);
	vertices[1].Set(0.0f, -1.5625f);
	vertices[2].Set(1.5625f, 0.0f);
	world.CreatePolyGround(0.625f, 10.9375f, vertices, 3);
	vertices[2].Set(-1.5625f, 0.0f);
	world.CreatePolyGround(24.375f, 10.9375f, vertices, 3);

	player = world.CreatePlayer(12.5f, 9.375f);
	ball = world.CreateBall(12.5f, 6.25f, 0.15625);

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
	const int32 velocityIterations = 10;
	const int32 positionIterations = 8;
	dTime += clock.Time();
	while (dTime >= C::TICK)
	{
		world.Update();
		dTime -= C::TICK;
		if (dTime < C::TICK)
			Render();
	}
}

void TestScene::HandleInput()
{
	if (pInput->VK[VK_LEFT])
		player->move_state = -1;
	if (pInput->VK[VK_RIGHT])
		player->move_state = 1;

	if (!pInput->VK[VK_LEFT] && !pInput->VK[VK_RIGHT])
		player->move_state = 0;
	else if (pInput->VK[VK_LEFT] && pInput->VK[VK_RIGHT])
	{
		if (pInput->keycode == VK_LEFT)
			player->move_state = -1;
		else if (pInput->keycode == VK_RIGHT)
			player->move_state = 1;
	}
	else if (pInput->VK[VK_LEFT])
		player->move_state = -1;
	else if (pInput->VK[VK_RIGHT])
		player->move_state = 1;
	player->jump = pInput->VK[VK_UP];
	player->fpush = pInput->VK[VK_SPACE];
	player->fpull = pInput->VK[VK_CONTROL];
}

void TestScene::Render()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	d3ddev->BeginScene();

	debugdraw.Draw(*world.GetWorld(), camera);

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}