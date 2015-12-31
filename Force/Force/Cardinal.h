#pragma once

struct Input;
class Scene;

namespace Cardinal
{
	extern Input* pInput;

	bool Initialize(HWND hwnd);
	void Uninitialize();
	void End();

	void SetState(int index, void* state);
	void* GetState(int index);

	bool RegisterInputDevices();
	void BufferInput(HRAWINPUT handle);
	void HandleBufferedInput();

	int LoadScene(Scene* ps);
	void NextScene(Scene* ps);
	void UpdateScene();
	void RenderScene();
	void ReleaseScene();
}