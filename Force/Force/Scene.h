#pragma once

#include "Cardinal.h"
#include "Audio.h"
#include "Graphics.h"
#include "Misc.h"

class Subscene;
class View;
class ViewManager;

typedef LList<View> ViewList;

class Scene
{
public:
	Scene()
	{
		d3d = Graphics::d3d;
		d3ddev = Graphics::d3ddev;
		sprite = Graphics::sprite;
		pInput = Cardinal::pInput;
	}
	virtual void Release() = 0; //must delete itself
	virtual void Update() = 0;
	virtual void HandleInput() = 0;
	virtual void Render() = 0;

	ViewManager* VM = NULL;

protected:
	IDirect3D9* d3d;
	IDirect3DDevice9* d3ddev;
	ID3DXSprite* sprite;
	Input* pInput;
	Subscene** pScenes;
};

class Subscene : public Scene
{
public:
	Subscene() : Scene() {}
	Subscene(Scene* pScene) : Scene() { VM = pScene->VM; }

protected:
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bActive = true;
	bool bVisible = true;
};

class View : public Subscene
{
public:
	View() : Subscene() {}
	View(Subscene* pScene) : Subscene() { VM = pScene->VM; }

protected:
	//Stuff...
};

class ViewManager
{
public:
	void Release(); //must delete itself and all views
	void Update();
	void HandleInput();
	void Render();

	//Stuff like add, bring-to-front, destroy, etc.
	void AddView(View* view);

private:
	ViewList viewList;
};