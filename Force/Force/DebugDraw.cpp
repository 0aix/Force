#include "stdafx.h"
#include "Draw.h"

DebugDraw::DebugDraw()
{
	d3ddev = Graphics::d3ddev;
	if (FAILED(d3ddev->CreateVertexBuffer(MAX_VERTICES * sizeof(VERTEX_2D_DIF), 0, VERTEX_2D_DIF::DIF2DFVF, D3DPOOL_DEFAULT, &vertex_buffer, NULL)))
		exit(1);
	for (int i = 0; i < MAX_VERTICES; i++)
	{
		vertices[i].z = 0.0f;
		vertices[i].rhw = 1.0f;
	}
	if (FAILED(D3DXCreateFontA(d3ddev, 14, 0, 450, 0, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ARIAL", &font)))
		exit(1);
	clock.Start();
}

DebugDraw::~DebugDraw()
{
	vertex_buffer->Release();
	font->Release();
}

//call this to start debug draw
//callbacks will force render whenever the vertex buffer is full
void DebugDraw::Draw(b2World& world, const Camera& camera)
{
	m_camera = camera;
	world.DrawDebugData();
	Render();
	FPS();
}

void DebugDraw::Render()
{
	//Set vertex format and stream source
	d3ddev->SetFVF(VERTEX_2D_DIF::DIF2DFVF);
	d3ddev->SetStreamSource(0, vertex_buffer, 0, sizeof(VERTEX_2D_DIF));

	//Copy over vertex data
	VERTEX_2D_DIF* buffer;
	UINT size = vertex_count * sizeof(VERTEX_2D_DIF);
	if (FAILED(vertex_buffer->Lock(0, size, reinterpret_cast<void**>(&buffer), 0)))
		return;
	memcpy(buffer, vertices, size);
	if (FAILED(vertex_buffer->Unlock()))
		exit(1);

	//Iterate through the vector and draw the primitives
	DPI dpi;
	vector<DPI>::iterator end = dpi_list.end();
	for (vector<DPI>::iterator it = dpi_list.begin(); it != end; it++)
	{
		dpi = *it;
		d3ddev->DrawPrimitive(dpi.PrimitiveType, dpi.StartVertex, dpi.PrimitiveCount);
	}
	//Clear primitives info
	dpi_list.clear();
	vertex_count = 0;
}

void DebugDraw::FPS()
{
	RECT rect = { 5, 5, 100, 20 }; //left, top, right, bottom
	if (++framecount >= 30)
	{
		double elapsed = clock.Time();
		if (elapsed > 0.0)
		{
			fps = 30.0 / elapsed;
			sprintf_s(fpsbuffer, "FPS: %lf", fps);
			totalframecount += 30.0;
			totalelapsed += elapsed;
			afps = totalframecount / totalelapsed;
			sprintf_s(afpsbuffer, "AFPS: %lf", afps);
		}
		framecount = 0;
	}
	font->DrawTextA(NULL, fpsbuffer, 16, &rect, 0, 0xFF00FF00); //green font
	rect.top += 15;
	rect.bottom += 15;
	font->DrawTextA(NULL, afpsbuffer, 16, &rect, 0, 0xFFFFA500); //orange font
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	if (vertex_count + vertexCount + 1 > MAX_VERTICES)
		Render();

	DPI dpi;
	dpi.PrimitiveCount = vertexCount;
	dpi.StartVertex = vertex_count;
	dpi.PrimitiveType = D3DPT_LINESTRIP;
	dpi_list.push_back(dpi);

	VERTEX_2D_DIF* buffer = this->vertices;
	Camera camera = m_camera;
	DWORD argb = D3DCOLOR_ARGB(static_cast<DWORD>(color.a * 255.0f), static_cast<DWORD>(color.r * 255.0f),
		static_cast<DWORD>(color.g * 255.0f), static_cast<DWORD>(color.b * 255.0f));
	int n;
	for (int i = 0; i < vertexCount; i++)
	{
		n = dpi.StartVertex + i;
		buffer[n].x = (vertices[i].x - camera.x) * camera.scale;
		buffer[n].y = (vertices[i].y - camera.y) * camera.scale;
		buffer[n].color = argb;
	}
	n++;
	buffer[n].x = (vertices[0].x - camera.x) * camera.scale;
	buffer[n].y = (vertices[0].y - camera.y) * camera.scale;
	buffer[n].color = argb;

	//update the vertex count
	vertex_count += vertexCount + 1;
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	if (vertex_count + vertexCount > MAX_VERTICES)
		Render();

	DPI dpi;
	dpi.PrimitiveCount = vertexCount - 2;
	dpi.StartVertex = vertex_count;
	dpi.PrimitiveType = D3DPT_TRIANGLEFAN;
	dpi_list.push_back(dpi);

	VERTEX_2D_DIF* buffer = this->vertices;
	Camera camera = m_camera;
	DWORD argb = D3DCOLOR_ARGB(static_cast<DWORD>(color.a * 127.5f), static_cast<DWORD>(color.r * 255.0f),
		static_cast<DWORD>(color.g * 255.0f), static_cast<DWORD>(color.b * 255.0f));
	int n;
	for (int i = 0; i < vertexCount; i++)
	{
		n = dpi.StartVertex + i;
		buffer[n].x = (vertices[i].x - camera.x) * camera.scale;
		buffer[n].y = (vertices[i].y - camera.y) * camera.scale;
		buffer[n].color = argb;
	}

	//update the vertex count
	vertex_count += vertexCount;

	//draw polygon outline
	DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	Camera camera = m_camera;
	float r = radius * camera.scale;
	const int vertexCount = 36;
	if (vertex_count + vertexCount + 1 > MAX_VERTICES)
	{
		Render();
		if (vertexCount + 1 > MAX_VERTICES)
			return;
	}

	DPI dpi;
	dpi.PrimitiveCount = vertexCount;
	dpi.StartVertex = vertex_count;
	dpi.PrimitiveType = D3DPT_LINESTRIP;
	dpi_list.push_back(dpi);

	VERTEX_2D_DIF* buffer = this->vertices;
	DWORD argb = D3DCOLOR_ARGB(static_cast<DWORD>(color.a * 255.0f), static_cast<DWORD>(color.r * 255.0f),
		static_cast<DWORD>(color.g * 255.0f), static_cast<DWORD>(color.b * 255.0f));
	int n;
	float step = 2.0f * b2_pi / static_cast<float>(vertexCount);
	float radian = 0.0f;
	float center_x = (center.x - camera.x) * camera.scale;
	float center_y = (center.y - camera.y) * camera.scale;
	for (int i = 0; i < vertexCount; i++)
	{
		n = dpi.StartVertex + i;
		buffer[n].x = center_x + r * cos(radian);
		buffer[n].y = center_y + r * sin(radian);
		buffer[n].color = argb;
		radian += step;
	}
	n++;
	buffer[n].x = center_x + r * cos(radian);
	buffer[n].y = center_y + r * sin(radian);
	buffer[n].color = argb;
	//update the vertex count
	vertex_count += vertexCount + 1;
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	Camera camera = m_camera;
	float r = radius * camera.scale;
	const int vertexCount = 36;
	if (vertex_count + vertexCount > MAX_VERTICES)
	{
		Render();
		if (vertexCount > MAX_VERTICES)
			return;
	}

	DPI dpi;
	dpi.PrimitiveCount = vertexCount - 2;
	dpi.StartVertex = vertex_count;
	dpi.PrimitiveType = D3DPT_TRIANGLEFAN;
	dpi_list.push_back(dpi);

	VERTEX_2D_DIF* buffer = this->vertices;
	DWORD argb = D3DCOLOR_ARGB(static_cast<DWORD>(color.a * 127.5f), static_cast<DWORD>(color.r * 255.0f),
		static_cast<DWORD>(color.g * 255.0f), static_cast<DWORD>(color.b * 255.0f));
	int n;
	float step = 2.0f * b2_pi / static_cast<float>(vertexCount);
	float radian = 0.0f;
	float center_x = (center.x - camera.x) * camera.scale;
	float center_y = (center.y - camera.y) * camera.scale;
	for (int i = 0; i < vertexCount; i++)
	{
		n = dpi.StartVertex + i;
		buffer[n].x = center_x + r * cos(radian);
		buffer[n].y = center_y + r * sin(radian);
		buffer[n].color = argb;
		radian += step;
	}

	//update the vertex count
	vertex_count += vertexCount;

	//draw circle outline
	DrawCircle(center, radius, color);

	//draw circle axis
	DrawSegment(center, center + radius * axis, color);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	if (vertex_count + 2 > MAX_VERTICES)
		Render();

	DPI dpi;
	dpi.PrimitiveCount = 1;
	dpi.StartVertex = vertex_count;
	dpi.PrimitiveType = D3DPT_LINESTRIP;
	dpi_list.push_back(dpi);

	VERTEX_2D_DIF* buffer = this->vertices;
	Camera camera = m_camera;
	DWORD argb = D3DCOLOR_ARGB(static_cast<DWORD>(color.a * 255.0f), static_cast<DWORD>(color.r * 255.0f),
		static_cast<DWORD>(color.g * 255.0f), static_cast<DWORD>(color.b * 255.0f));
	int n = dpi.StartVertex;
	buffer[n].x = (p1.x - camera.x) * camera.scale;
	buffer[n].y = (p1.y - camera.y) * camera.scale;
	buffer[n].color = argb;
	n++;
	buffer[n].x = (p2.x - camera.x) * camera.scale;
	buffer[n].y = (p2.y - camera.y) * camera.scale;
	buffer[n].color = argb;

	//update the vertex count
	vertex_count += 2;
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	const float32 axis_scale = 0.4f;
	b2Color red(1.0f, 0.0f, 0.0f);
	b2Color green(0.0f, 1.0f, 0.0f);
	b2Vec2 p1 = xf.p, p2;
	p2 = p1 + axis_scale * xf.q.GetXAxis();
	DrawSegment(p1, p2, red);
	p2 = p1 + axis_scale * xf.q.GetYAxis();
	DrawSegment(p1, p2, green);
}