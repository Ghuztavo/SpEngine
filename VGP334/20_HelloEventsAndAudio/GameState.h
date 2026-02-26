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

	void OnSpacePressedEvent(const SpEngine::Core::Event& e);

	SpEngine::Graphics::Camera mCamera;
	SpEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	SpEngine::Physics::ParticleSystem mParticleSystem;
	SpEngine::Physics::ParticleSystem mFireworkParticles;

	SpEngine::Core::EventListenerId mSpacePressedListenerId = 0;
	SpEngine::Core::EventListenerId mEnterPressedListenerId = 0;

	SpEngine::Graphics::Animation mFireworkAnimation;
	float mFireworkAnimationTime = 0.0f;

	SpEngine::Audio::SoundId mLaunchSoundId = 0;
	SpEngine::Audio::SoundId mExplosionSoundId = 0;

};