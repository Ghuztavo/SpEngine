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
	SpEngine::Graphics::Animator mAnimator;

	SpEngine::Graphics::StandardEffect mStandardEffect;

	int mClipIndex = -1;
	float mAnimationSpeed = 1.0f;
	bool mDrawSkeleton = false;
	
};