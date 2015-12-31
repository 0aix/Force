#include "stdafx.h"

#ifndef _DEBUG
#pragma comment(lib, "Box2D.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "libogg.lib")
#pragma comment(lib, "libvorbis.lib")
#pragma comment(lib, "libvorbisfile.lib")
#else
#pragma comment(lib, "Box2Dd.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9d.lib")
#pragma comment(lib, "liboggd.lib")
#pragma comment(lib, "libvorbisd.lib")
#pragma comment(lib, "libvorbisfiled.lib")
#endif