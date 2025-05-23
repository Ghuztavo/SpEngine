#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;

namespace
{
	Color GetNextColor(int& index)
	{
		constexpr Color colorTable[] =
		{
			Colors::Red,
			Colors::Green,
			Colors::Blue,
			Colors::Yellow,
			Colors::Cyan,
			Colors::Magenta,
			Colors::White,
			Colors::Pink
		};

		index = (index + 1) % std::size(colorTable);
		return colorTable[index];
	}

	void CreateCubeIndices(std::vector<uint32_t>& indices)
	{
		indices = {
			//front
			0, 1, 2,
			0, 2, 3,
			//back
			7, 5, 4,
			7, 6, 5,
			//right
			3, 2, 6,
			3, 6, 7,
			//left
			4, 5, 1,
			4, 1, 0,
			//top
			1, 5, 6,
			1, 6, 2,
			//bottom
			0, 3, 7,
			0, 7, 4
		};
	}

	void CreatePlaneIndices(std::vector<uint32_t>& indices, int numRows, int numColumns)
	{
		for (int r = 0; r < numRows; ++r)
		{
			for (int c = 0; c < numColumns; ++c)
			{
				int i = (r * (numColumns + 1)) + c;

				//triangle 0
				indices.push_back(i);
				indices.push_back(i + numColumns + 1);
				indices.push_back(i + numColumns + 2);

				//triangle 1
				indices.push_back(i);
				indices.push_back(i + numColumns + 2);
				indices.push_back(i + 1);
			}
		}
	}

	void CreateCapIndices(std::vector<uint32_t>& indices, int slices, int topIndex, int bottomIndex)
	{
		for (int s = 0; s < slices; ++s)
		{
			//bottom triangle
			indices.push_back(bottomIndex);
			indices.push_back(s);
			indices.push_back(s + 1);

			//top triangle
			int topRowIndex = topIndex - slices - 1 + s;
			indices.push_back(topIndex);
			indices.push_back(topRowIndex + 1);
			indices.push_back(topRowIndex);
		}
	}
}

MeshPC MeshBuilder::CreateCubePC(float size, const Color& color)
{
	MeshPC mesh;


	const float hs = 0.5f;
	mesh.vertices.push_back({ { -hs, -hs, -hs }, color });
	mesh.vertices.push_back({ { -hs,  hs, -hs }, color });
	mesh.vertices.push_back({ {  hs,  hs, -hs }, color });
	mesh.vertices.push_back({ {  hs, -hs, -hs }, color });

	//back
	mesh.vertices.push_back({ { -hs, -hs, hs }, color });
	mesh.vertices.push_back({ { -hs,  hs, hs }, color });
	mesh.vertices.push_back({ {  hs,  hs, hs }, color });
	mesh.vertices.push_back({ {  hs, -hs, hs }, color });

	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
	MeshPC mesh;

	int index = rand() % 100;

	const float hs = 0.5f;
	//front
	mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hs,  hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hs,  hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });

	//back
	mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hs,  hs, hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hs,  hs, hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hs, -hs, hs }, GetNextColor(index) });

	CreateCubeIndices(mesh.indices);

	return mesh;
}

//MeshPX MeshBuilder::CreateCubePX(float size)
//{
//	MeshPX mesh;
//	const float hs = 0.5f;
//	mesh.vertices.push_back({ { -hs, -hs, -hs }, { 0.0f, 0.0f } });
//	mesh.vertices.push_back({ { -hs,  hs, -hs }, { 0.0f, 1.0f } });
//	mesh.vertices.push_back({ {  hs,  hs, -hs }, { 1.0f, 1.0f } });
//	mesh.vertices.push_back({ {  hs, -hs, -hs }, { 1.0f, 0.0f } });
//	//back
//	mesh.vertices.push_back({ { -hs, -hs, hs }, { 0.0f, 0.0f } });
//	mesh.vertices.push_back({ { -hs,  hs, hs }, { 0.0f, 1.0f } });
//	mesh.vertices.push_back({ {  hs,  hs, hs }, { 1.0f, 1.0f } });
//	mesh.vertices.push_back({ {  hs, -hs, hs }, { 1.0f, 0.0f } });
//	CreateCubeIndices(mesh.indices);
//	return mesh;
//
//}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
	MeshPC mesh;

	int index = rand() % 100;
	const float hs = 0.5f;
	// add all the vertices for the pyramid
	//front face
	mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {   0,  hs,   0 }, GetNextColor(index) });// top
	mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });

	//back
	mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hs, -hs, hs }, GetNextColor(index) });

	// add all the indices for the pyramid
	mesh.indices = {
		//front
		0, 1, 2,
		//back
		3, 4, 1,
		//right
		2, 1, 4,
		//left
		0, 3, 1,
		//bottom
		0, 2, 4,
		0, 4, 3
	};

	return mesh;
}

MeshPC MeshBuilder::CreateRectanglePC(float width, float height, float depth)
{
	MeshPC mesh;

	int index = rand() % 100;
	const float hw = width * 0.5f;
	const float hh = height * 0.5f;
	const float hd = depth * 0.5f;

	// add all the vertices for the rectangle
	//front face
	mesh.vertices.push_back({ { -hw, -hh, -hd }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hw,  hh, -hd }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw,  hh, -hd }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw, -hh, -hd }, GetNextColor(index) });

	//back face
	mesh.vertices.push_back({ { -hw, -hh,  hd }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hw,  hh,  hd }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw,  hh,  hd }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw, -hh,  hd }, GetNextColor(index) });

	// add all the indices for the rectangle
	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPC SpEngine::Graphics::MeshBuilder::CreatePlanePC(int numRows, int numCols, float spacing, bool horizontal)
{
	MeshPC mesh;
	int index = rand() % 100;
	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;

	float w = -hpw;
	float h = -hph;
	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c <= numCols; ++c)
		{
			Math::Vector3 pos = (horizontal) ? Math::Vector3(w, 0.0f, h ) : Math::Vector3( w, h, 0.0f );
			mesh.vertices.push_back({ pos, GetNextColor(index) });
			w += spacing;
		}
		w = -hpw;
		h += spacing;

	}

	CreatePlaneIndices(mesh.indices, numRows, numCols);

	return mesh;
}

MeshPX SpEngine::Graphics::MeshBuilder::CreatePlanePX(int numRows, int numCols, float spacing, bool horizontal)
{
	MeshPX mesh;

	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;
	const float uInc = 1.0f / static_cast<float>(numCols);
	const float vInc = 1.0f / static_cast<float>(numRows);

	float w = -hpw;
	float h = -hph;
	float u = 0.0f;
	float v = 1.0f;
	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c <= numCols; ++c)
		{
			Math::Vector3 pos = (horizontal) ? Math::Vector3(w, 0.0f, h) : Math::Vector3(w, h, 0.0f);
			mesh.vertices.push_back({ pos, {u, v } });
			w += spacing;
			u += uInc;
		}
		w = -hpw;
		h += spacing;
		u = 0.0f;
		v -= vInc;

	}

	CreatePlaneIndices(mesh.indices, numRows, numCols);

	return mesh;
}

MeshPC SpEngine::Graphics::MeshBuilder::CreateCylinderPC(int slices, int rings)
{
	MeshPC mesh;
	int index = rand() % 100;

	const float hh = static_cast<float>(rings) * 0.5f;
	const float fSlices = static_cast<float>(slices);
	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = (slice / fSlices) * Math::Constants::TwoPi;

			mesh.vertices.push_back({ {
					sin(rotation),
					ring - hh,
					-cos(rotation)},
				GetNextColor(index) });
		}
	}

	mesh.vertices.push_back({ { 0.0f, hh, 0.0f }, GetNextColor(index) });
	mesh.vertices.push_back({ { 0.0f, -hh, 0.0f }, GetNextColor(index) });

	CreatePlaneIndices(mesh.indices, rings, slices);
	CreateCapIndices(mesh.indices, slices, mesh.vertices.size() - 2, mesh.vertices.size() - 1);


	return mesh;
}

MeshPC SpEngine::Graphics::MeshBuilder::CreateSpherePC(int slices, int rings, float radius)
{
	MeshPC mesh;
	int index = rand() % 100;

	float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
	float horRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * vertRotation;
		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = slice * horRotation;
			mesh.vertices.push_back({ {
					radius * sin(phi) * sin(rotation),
					radius * cos(phi),
					radius * sin(phi) * cos(rotation)},
				GetNextColor(index) });
		}

	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(int slices, int rings, float radius)
{
	MeshPX mesh;
	float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
	float horRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);

	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * vertRotation;
		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = slice * horRotation;

			float u = uStep * slice;
			float v = vStep * ring;
			mesh.vertices.push_back({ {
					radius * sin(phi) * sin(rotation),
					radius * cos(phi),
					radius * sin(phi) * cos(rotation)},
					{u, v} });
		}

	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}

MeshPX SpEngine::Graphics::MeshBuilder::CreateSkyBoxSpherePX(int slices, int rings, float radius)
{
	MeshPX mesh;
	float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
	float horRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);

	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * vertRotation;
		for (int s = 0; s <= slices; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = slice * horRotation;

			float u = uStep * slice;
			float v = vStep * ring;
			mesh.vertices.push_back({ {
					radius * sin(phi) * cos(rotation),
					radius * cos(phi),
					radius * sin(phi) * sin(rotation)},
					{u, v} });
		}

	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}

