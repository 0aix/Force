#include "stdafx.h"
#include "Game.h"
#include "Audio.h"
#include "Graphics.h"
#include "Scenes.h"

namespace Game
{
	HANDLE audio;
	HANDLE update;
	HANDLE render;
	bool bExit = false;

	bool Initialize(HWND hwnd)
	{
		if (!Audio::Initialize())
			return false;

		if (!Graphics::Initialize(hwnd))
			return false;

		/////////////////////////////////

		Cardinal::NextScene(new TestScene());

		/////////////////////////////////

		audio = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AudioThread, NULL, 0, NULL);
		update = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateThread, NULL, 0, NULL);
		render = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RenderThread, NULL, 0, NULL);

		if (!update || !render)
			return false;

		return true;
	}

	void Uninitialize()
	{
		bExit = true;
		HANDLE handle[3] = { audio, update, render };
		WaitForMultipleObjects(3, handle, true, INFINITE);
		CloseHandle(audio);
		CloseHandle(update);
		CloseHandle(render);

		Cardinal::ReleaseScene();
		Audio::Uninitialize();
		Graphics::Uninitialize();
	}

	void AudioThread()
	{
		for (; !bExit; Sleep(100))
			Audio::Update();
	}

	void UpdateThread()
	{
		for (; !bExit; Sleep(1))
			Cardinal::UpdateScene();
	}

	void RenderThread()
	{
		//for (; !bExit; Sleep(1))
		//	Cardinal::RenderScene();
	}
}