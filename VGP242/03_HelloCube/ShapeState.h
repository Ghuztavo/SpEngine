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
	
	using Vertices = std::vector<SpEngine::Graphics::VertexPC>;
	Vertices mVertices;

	SpEngine::Graphics::MeshBuffer mMeshBuffer;
	SpEngine::Graphics::VertexShader mVertexShader;
	SpEngine::Graphics::PixelShader mPixelShader;
};
