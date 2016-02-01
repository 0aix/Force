#pragma once

#include "Windows.h"

#define RI_LEFT_FLAG 0x01
#define RI_LEFT_EVENT 0x02
#define RI_LEFT_DOWN(x) (x & RI_LEFT_FLAG)
#define RI_LEFT_UP(x) (!RI_LEFT_DOWN(x))
#define RI_LEFT_KEY_DOWN(x) ((x & RI_LEFT_EVENT) && RI_LEFT_DOWN(x))
#define RI_LEFT_KEY_UP(x) ((x & RI_LEFT_EVENT) && RI_LEFT_UP(x))
#define RI_RIGHT_FLAG 0x04
#define RI_RIGHT_EVENT 0x08
#define RI_RIGHT_DOWN(x) (x & RI_RIGHT_FLAG)
#define RI_RIGHT_UP(x) (!RI_RIGHT_DOWN(x))
#define RI_RIGHT_KEY_DOWN(x) ((x & RI_RIGHT_EVENT) && RI_RIGHT_DOWN(x))
#define RI_RIGHT_KEY_UP(x) ((x & RI_RIGHT_EVENT) && RI_RIGHT_UP(x))
#define RI_MIDDLE_FLAG 0x10
#define RI_MIDDLE_EVENT 0x20
#define RI_MIDDLE_DOWN(x) (x & RI_MIDDLE_FLAG)
#define RI_MIDDLE_UP(x) (!RI_MIDDLE_DOWN(x))
#define RI_MIDDLE_KEY_DOWN(x) ((x & RI_MIDDLE_EVENT) && RI_MIDDLE_DOWN(x))
#define RI_MIDDLE_KEY_UP(x) ((x & RI_MIDDLE_EVENT) && RI_MIDDLE_UP(x))
#define RI_WHEEL_FLAG 0x40

struct Input
{
	byte type; //0 == mouse, 1 == keyboard

	POINT pos;
	POINT vel;
	byte buttons;
	short delta;

	byte keycode;
	bool keystate; //true == down, false == up
	bool VK[146];
	//maybe #define VK[CTRL] VK[146] and etc.
	bool CTRL;
	bool SHIFT;
	bool ALT;
	bool CLR;
};