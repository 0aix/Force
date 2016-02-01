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
	dTime += clock.Time();
	while (dTime >= C::TICK)
	{
		world.Update();
		dTime -= C::TICK;
		if (dTime < C::TICK)
			Render();
		if (reset)
		{
			Cardinal::NextScene(new TestScene());
			Release();
			return;
		}
	}
}

void TestScene::HandleInput()
{
	if (pInput->CLR)
	{

	}
	else if (pInput->type == RIM_TYPEMOUSE)
	{
		byte buttons = pInput->buttons;
		player->fpull = RI_RIGHT_KEY_DOWN(buttons);
		if (player->hasball && !player->fpass)
		{
			if (RI_LEFT_KEY_DOWN(buttons))
				player->FReady();
		}
		else if (player->hasball && player->fpass)
		{
			player->FCharge(pInput->vel.x, pInput->vel.y);
			if (RI_LEFT_KEY_UP(buttons))
				player->FPass();
		}
	}
	else //pInput->type == RIM_TYPEKEYBOARD
	{
		if (pInput->VK['A'])
			player->move_state = -1;
		if (pInput->VK['D'])
			player->move_state = 1;

		if (!pInput->VK['A'] && !pInput->VK['D'])
			player->move_state = 0;
		else if (pInput->VK['A'] && pInput->VK['D'])
		{
			if (pInput->keycode == 'A')
				player->move_state = -1;
			else if (pInput->keycode == 'D')
				player->move_state = 1;
		}
		else if (pInput->VK['A'])
			player->move_state = -1;
		else if (pInput->VK['D'])
			player->move_state = 1;
		player->jump = pInput->VK['W'];
		player->fpush = pInput->VK[VK_SPACE];

		reset = pInput->VK[VK_ESCAPE];
	}
}

void TestScene::Render()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	d3ddev->BeginScene();

	debugdraw.Draw(*world.GetWorld(), camera);

	/*POINT pos = pInput->pos;

	int indices[4] = { 0, 1, 2, 3 };
	VERTEX_2D_DIF vertices[4];
	vertices[0].x = pos.x - 5.0f;
	vertices[0].y = pos.y - 5.0f;
	vertices[1].x = pos.x + 5.0f;
	vertices[1].y = pos.y - 5.0f;
	vertices[2].x = pos.x + 5.0f;
	vertices[2].y = pos.y + 5.0f;
	vertices[3].x = pos.x - 5.0f;
	vertices[3].y = pos.y + 5.0f;
	d3ddev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLEFAN, 0, 4, 2, indices, D3DFMT_INDEX32, vertices, sizeof(VERTEX_2D_DIF));*/

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}