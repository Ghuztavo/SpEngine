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
	void UpdateCamera(float deltaTime);

	SpEngine::Graphics::Camera mCamera;
	SpEngine::Graphics::Camera mRenderTargetCamera;

	//GPU communication
	SpEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	//render object
	SpEngine::Graphics::SimpleTextureEffect::RenderData mObject0;
	SpEngine::Graphics::SimpleTextureEffect::RenderData mObject1;

	//render target
	SpEngine::Graphics::RenderTarget mRenderTarget;
};