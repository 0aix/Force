#pragma once

#include <d3dx9.h>
#include "Constants.h"
#include "Misc.h"

template <typename T>
struct Anim
{
	T* items;
	double* delays;
	int count;
	bool loop;

	Anim<T>() {}
	Anim<T>(int c, bool b)
	{
		items = new T[c];
		count = c;
		loop = b;
	}
};

template <typename T>
struct Animation
{
	Anim<T> anim;
	int index = 0;
	double acc = 0.0;
	double delay;
	double mult = 1.0;

	Animation<T>() {}
	Animation<T>(Anim<T>* a)
	{
		anim = *a;
		delay = anim.delays[0];
	}

	//whether changed
	bool Update()
	{
		//could make a ready check here but guess not
		acc += mult * C::TICK;
		bool updated = false;
		while (acc >= delay)
		{
			acc -= delay;
			index++;
			if (index == count && loop)
				index = 0;
			delay = anim.delays[index];
			updated = true;
		}
		return updated;
	}

	T* Get() { return &anim.items[index]; }
};

struct Sprite
{
	IDirect3DTexture9* texture;
	D3DXVECTOR3 center;
	double delay;
};

struct SpriteAnim
{
	Sprite* sprites;
	int count;
	bool loop;

	SpriteAnim(int c, bool b)
	{
		sprites = new Sprite[c];
		count = c;
		loop = b;
	}
};

struct SpriteAnimation
{
	Sprite* sprites = NULL;
	double acc = 0.0;
	int index = 0;
	int count;
	bool loop;
	double delay;
	double mult = 1.0;

	SpriteAnimation(SpriteAnim* anim)
	{
		//assume sprites is filled with complete sprites
		//note to future brian: forgot what above comment means
		sprites = anim->sprites;
		count = anim->count;
		loop = anim->loop;
		delay = sprites[0].delay;
	}

	//whether sprite changed
	bool Update()
	{
		//could make a ready check here but.. naahhh
		acc += mult * C::TICK;
		bool update = false;
		while (acc >= delay)
		{
			acc -= delay;
			index++;
			update = true;
			if (index == count && loop)
				index = 0;
			delay = sprites[index].delay;
		}
		return update;
	}

	//well.. tech not needed but less referencing, should be faster
	Sprite* Render() { return &sprites[index]; }
};

struct Effect
{
	SpriteAnimation animation;
	Sprite* sprite = NULL;
	D3DXVECTOR3 pos;
	double radian = 0.0; //**
};

typedef LList<Effect> EffectList;