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

	SpEngine::Graphics::StandardEffect mStandardEffect;
	
};