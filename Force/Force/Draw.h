#pragma once

class Camera
{
public:
	float x;
	float y;
	float scale;
};

//Implementation in DebugDraw.cpp
class DebugDraw : public b2Draw
{
public:
	DebugDraw();
	~DebugDraw();
	void Draw(b2World& world, const Camera& camera);
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);

private:
	static const int MAX_VERTICES = 1024;

	IDirect3DDevice9* d3ddev;
	VERTEX_2D_DIF vertices[MAX_VERTICES];
	Camera m_camera;
	struct DPI //DrawPrimitive Info
	{
		D3DPRIMITIVETYPE PrimitiveType;
		UINT StartVertex;
		UINT PrimitiveCount;
	};
	IDirect3DVertexBuffer9* vertex_buffer;
	vector<DPI> dpi_list = vector<DPI>(256);
	int vertex_count = 0;

	Clock clock;
	ID3DXFont* font;
	int framecount = 0;
	double fps = 0.0;
	char fpsbuffer[256];
	double totalframecount = 0.0;
	double totalelapsed = 0.0;
	double afps = 0;
	char afpsbuffer[256];

	void Render();
	void FPS();
};