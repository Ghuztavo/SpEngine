#pragma once

#include "CustomTypeIDs.h"
#include <SpEngine/Inc/SpEngine.h>

class WeaponComponent : public SpEngine::Component
{
public:
	SET_TYPE_ID(WeaponComponentId::Weapon);

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime);
	void DebugUI() override;
	void Deserialize(const rapidjson::Value& value) override;

	void RenderParticles(SpEngine::Graphics::ParticleSystemEffect& effect);

private:
	const SpEngine::TransformComponent* mTransformComponent = nullptr;
	const SpEngine::CameraComponent* mCameraComponent = nullptr;
	const SpEngine::RigidBodyComponent* mRigidBodyComponent = nullptr;
	SpEngine::Math::Vector3 mMuzzleOffset = SpEngine::Math::Vector3::Zero;
	SpEngine::Graphics::Color mBulletTrailColor = SpEngine::Graphics::Colors::Red;

	float mTimeBetweenShots = 0.5f;
	float mRange = 100.0f;
	float mFireCooldown = 0.0f;

	float mTraceDuration = 0.05f;
	float mTraceTimer = 0.0f;
	SpEngine::Math::Vector3 mTraceStart = SpEngine::Math::Vector3::Zero;
	SpEngine::Math::Vector3 mTraceEnd = SpEngine::Math::Vector3::Zero;

	std::string mParticleTextureFileName;
	std::string mShootSoundFileName;

	SpEngine::Graphics::TextureId mParticleTextureId = 0;
	std::size_t mShootSoundId = 0;

	SpEngine::Physics::ParticleSystem mHitParticles;
};