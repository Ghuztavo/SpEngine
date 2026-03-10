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

	SpEngine::Core::EventListenerId mSpacePressedListenerId = 0;
	SpEngine::Core::EventListenerId mEnterPressedListenerId = 0;

	SpEngine::Graphics::Camera mCamera;
	SpEngine::Graphics::StandardEffect mStandardEffect;
	SpEngine::Graphics::DirectionalLight mDirectionalLight;
	SpEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	SpEngine::Physics::ParticleSystem mElevatorCrashParticles;

	SpEngine::Graphics::RenderObject mGroundObject;
	SpEngine::Physics::CollisionShape mGroundShape;
	SpEngine::Physics::RigidBody mGroundRigidBody;

	SpEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	SpEngine::Graphics::SimpleTextureEffect::RenderData mSkySphere;

	struct ElevatorComponents
	{
		SpEngine::Graphics::RenderObject mElevatorFloor;
		SpEngine::Graphics::RenderObject mElevatorRightWall;
		SpEngine::Graphics::RenderObject mElevatorLeftWall;
		SpEngine::Graphics::RenderObject mElevatorCeiling;
		SpEngine::Graphics::RenderObject mElevatorBackWall;
		SpEngine::Graphics::RenderObject mElevatorRightDoor;
		SpEngine::Graphics::RenderObject mElevatorLeftDoor;
	};
	SpEngine::Graphics::RenderObject mElevatorButton;

	SpEngine::Graphics::Animation mCharacter1Animation;
	SpEngine::Graphics::Animation mCharacter2Animation;
	SpEngine::Graphics::Animation mElevatorAnimation;
	float mCharacter1AnimationTime = 0.0f;
	float mCharacter2AnimationTime = 0.0f;
	float mElevatorAnimationTime = 0.0f;

	SpEngine::Audio::SoundId mElevatorFallingSound = 0;
	SpEngine::Audio::SoundId mElevatorCrashSound = 0;
	SpEngine::Audio::SoundId mGangnamSound = 0;

};