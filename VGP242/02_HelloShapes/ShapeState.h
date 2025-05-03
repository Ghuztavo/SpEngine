#pragma once

#include <SpEngine/Inc/SpEngine.h>

class ShapeState : public SpEngine::AppState
{
public:
	void Initialize()override;
	void Terminate()override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	virtual void CreateShape();
	struct Vertex
	{
		SpEngine::Math::Vector3 position;
		SpEngine::Graphics::Color color;
	};
	using Vertices = std::vector<Vertex>;
	Vertices mVertices;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};

class TriangleShapeState : public ShapeState
{
public:
	void Update(float deltaTime) override;
protected:
	void CreateShape() override;
};