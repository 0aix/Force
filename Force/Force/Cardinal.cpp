//PRECOMPILED
#include "stdafx.h"
#include "Cardinal.h"
#include "Game.h"
#include "Misc.h"
#include "Scene.h"
//***********

namespace Cardinal
{
	void** pStates;
	Input* pInput;
	Scene* pScene;
	RAWINPUT* pBuffer;

	HWND hWnd;
	mutex mtx;
	const int STATE_COUNT = 1000;
	const int RAWINPUT_BUFFER_SIZE = 16;
	int ri_start = 0;
	int ri_end = 0;

	bool Initialize(HWND hwnd)
	{
		pStates = new void*[STATE_COUNT];
		pInput = new Input{};
		pBuffer = new RAWINPUT[RAWINPUT_BUFFER_SIZE];

		hWnd = hwnd;

		if (!Game::Initialize(hwnd))
			return false;

		return true;
	}

	void Uninitialize()
	{
		Game::Uninitialize(); //Releases pScene

		delete pBuffer;
		delete pInput;
		delete pStates;
	}

	void End()
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}

	void SetState(int index, void* state)
	{
		if (0 <= index && index < STATE_COUNT)
			pStates[index] = state;
	}

	void* GetState(int index)
	{
		return pStates[index];
	}

	bool RegisterInputDevices()
	{
		RAWINPUTDEVICE RID[2];
		RID[0].usUsagePage = 1;
		RID[0].usUsage = 2;
		RID[0].dwFlags = 0;
		RID[0].hwndTarget = hWnd;
		RID[1].usUsagePage = 1;
		RID[1].usUsage = 6;
		RID[1].dwFlags = 0;
		RID[1].hwndTarget = hWnd;
		return RegisterRawInputDevices(RID, 2, sizeof(RAWINPUTDEVICE));
	}

	void BufferInput(HRAWINPUT handle)
	{
		int diff = ri_end - ri_start + 1;
		if (diff != 0 && diff != RAWINPUT_BUFFER_SIZE)
		{
			DWORD dwSize = sizeof(RAWINPUT);
			GetRawInputData(handle, RID_INPUT, pBuffer + ri_end, (PUINT)&dwSize, sizeof(RAWINPUTHEADER));
			if (++ri_end == RAWINPUT_BUFFER_SIZE)
				ri_end = 0;
		}
	}

	void HandleBufferedInput()
	{
		if (pInput->CLR)
		{
			pScene->HandleInput();
			ZeroMemory(pInput, sizeof(Input));
			return;
		}

		RAWINPUT raw;
		int ri_limit = ri_end;
		while (ri_start != ri_limit)
		{
			raw = pBuffer[ri_start];
			if (raw.header.dwType == RIM_TYPEMOUSE)
			{
				pInput->type = RIM_TYPEMOUSE;
				GetCursorPos(&pInput->pos);
				ScreenToClient(hWnd, &pInput->pos);
				RAWMOUSE mouse = raw.data.mouse;
				if (mouse.usFlags ^ MOUSE_MOVE_ABSOLUTE)
				{
					pInput->vel.x = mouse.lLastX;
					pInput->vel.y = mouse.lLastY;
				}
				byte buttons = pInput->buttons & (RI_LEFT_FLAG | RI_RIGHT_FLAG | RI_MIDDLE_FLAG); //zero out event flags
				byte flags;
				if (flags = mouse.usButtonFlags & (RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_LEFT_BUTTON_UP))
					buttons = (buttons & 0xFC) | flags | RI_LEFT_EVENT;
				if (flags = mouse.usButtonFlags & (RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_UP))
					buttons = (buttons & 0xF3) | flags | RI_RIGHT_EVENT;
				if (flags = mouse.usButtonFlags & (RI_MOUSE_MIDDLE_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_UP))
					buttons = (buttons & 0xCF) | flags | RI_MIDDLE_EVENT;
				if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
				{
					buttons |= RI_WHEEL_FLAG;
					pInput->delta = (short)mouse.usButtonData; // / WHEEL_DELTA
				}
				pInput->buttons = buttons;
			}
			else if (raw.header.dwType == RIM_TYPEKEYBOARD)
			{
				pInput->type = RIM_TYPEKEYBOARD;
				RAWKEYBOARD keyboard = raw.data.keyboard;
				byte vk = keyboard.VKey;
				if (vk < 146)
				{
					bool flag = ~keyboard.Flags & 0x01;
					pInput->keycode = vk;
					pInput->keystate = flag;
					pInput->VK[vk] = flag;
					if (vk == VK_CONTROL)
						pInput->CTRL = flag;
					else if (vk == VK_SHIFT)
						pInput->SHIFT = flag;
					else if (vk == VK_MENU)
						pInput->ALT = flag;
				}
			}
			else continue;
			pScene->HandleInput();
			ri_start++;
			if (ri_start == RAWINPUT_BUFFER_SIZE)
				ri_start = 0;
		}
	}

	int LoadScene(Scene* ps)
	{
		return 0;
	}

	void NextScene(Scene* ps)
	{
		pScene = ps;
	}

	void UpdateScene()
	{
		mtx.lock();

		HandleBufferedInput();
		pScene->Update();

		mtx.unlock();
	}

	void RenderScene()
	{
		mtx.lock();

		pScene->Render();

		mtx.unlock();
	}

	void ReleaseScene()
	{
		pScene->Release();
	}
}