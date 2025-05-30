#pragma once

#include <SpEngine/Inc/SpEngine.h>

class GameState : public SpEngine::AppState 
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	struct Object
	{
		SpEngine::Math::Matrix4 worldMat = SpEngine::Math::Matrix4::Identity;
		SpEngine::Graphics::MeshBuffer meshBuffer;
		SpEngine::Graphics::TextureId textureId = 0;
	};

	void UpdateCamera(float deltaTime);
	void RenderObject(const Object& object, const SpEngine::Graphics::Camera& camera);


	SpEngine::Graphics::Camera mCamera;
	SpEngine::Graphics::Camera mRenderTargetCamera;

	//GPU communication
	SpEngine::Graphics::ConstantBuffer mTransformBuffer;
	SpEngine::Graphics::VertexShader mVertexShader;
	SpEngine::Graphics::PixelShader mPixelShader;
	SpEngine::Graphics::Sampler mSampler;

	//render object
	Object mObject;
	Object mObject2;
	Object mObject3;

	//render target
	SpEngine::Graphics::RenderTarget mRenderTarget;
};