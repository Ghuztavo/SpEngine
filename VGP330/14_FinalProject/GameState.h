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
	SpEngine::Graphics::DirectionalLight mDirectionalLight;

	SpEngine::Graphics::RenderGroup mCharacter;
	SpEngine::Graphics::RenderGroup mCharacter2;
	SpEngine::Graphics::RenderGroup mCharacter3;
	SpEngine::Graphics::RenderObject mRenderObject;
	SpEngine::Graphics::RenderObject mRenderObject2;
	SpEngine::Graphics::RenderObject mRenderObject3;

	SpEngine::Graphics::RenderObject mGround;
	SpEngine::Graphics::Terrain mTerrain;

	SpEngine::Graphics::StandardEffect mStandardEffect;
	SpEngine::Graphics::ShadowEffect mShadowEffect;
	SpEngine::Graphics::TerrainEffect mTerrainEffect;
	SpEngine::Graphics::PostProcessingEffect mPostProcessingEffect;

	SpEngine::Graphics::RenderTarget mRenderTarget;
	SpEngine::Graphics::RenderObject mScreenQuad;
	SpEngine::Graphics::Texture mGrainTexture;
	
};