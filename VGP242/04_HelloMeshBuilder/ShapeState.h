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
	
	SpEngine::Graphics::MeshPX mMesh;
	SpEngine::Graphics::Camera mCamera;
	SpEngine::Graphics::ConstantBuffer mTransformBuffer;
	SpEngine::Graphics::MeshBuffer mMeshBuffer;
	SpEngine::Graphics::VertexShader mVertexShader;
	SpEngine::Graphics::PixelShader mPixelShader;

	//new
	SpEngine::Graphics::Texture mTexture;
	SpEngine::Graphics::Sampler mSampler;
};
