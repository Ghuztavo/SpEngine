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

	// Container
	SpEngine::Graphics::RenderGroup mElevator;
	SpEngine::Graphics::Animation mElevatorAnimation;
	float mElevatorAnimationTime = 0.0f;

	SpEngine::Graphics::RenderObject mElevatorButton;
	SpEngine::Graphics::Animation mElevatorButtonAnimation;
	float mElevatorButtonAnimationTime = 0.0f;
	SpEngine::Graphics::RenderObject mElevatorButtonPanel;

	// Character 1
	SpEngine::Graphics::RenderGroup mCharacter1;
	SpEngine::Graphics::Animation mCharacter1Animation;
	SpEngine::Graphics::Animator mCharacter1Animator;
	int mCharacter1ClipIndex = -1;
	float mCharacter1AnimationTime = 0.0f;
	void UpdateCharacter1Animator(float animationTime);

	// Character 2
	SpEngine::Graphics::RenderGroup mCharacter2;
	SpEngine::Graphics::Animation mCharacter2Animation;
	SpEngine::Graphics::Animator mCharacter2Animator;
	int mCharacter2ClipIndex = -1;
	float mCharacter2AnimationTime = 0.0f;
	void UpdateCharacter2Animator(float animationTime);

	SpEngine::Physics::ParticleSystem mCrashingParticles;
	SpEngine::Graphics::Animation mCrashingAnimation;
	float mCrashingAnimationTime = 0.0f;


	SpEngine::Audio::SoundId mWalkingSound = 0;
	SpEngine::Audio::SoundId mElevatorFallingSound = 0;
	SpEngine::Audio::SoundId mElevatorCrashSound = 0;
	SpEngine::Audio::SoundId mBrickSound = 0;
	SpEngine::Audio::SoundId mGangnamSound = 0;

};