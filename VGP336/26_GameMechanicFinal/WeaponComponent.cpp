#include "WeaponComponent.h"
#include "WeaponService.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Math;

void WeaponComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	GameObject* parent = GetOwner().GetParent();
	if (parent != nullptr)
	{
		mCameraComponent = parent->GetComponent<CameraComponent>();
		mRigidBodyComponent = parent->GetComponent<RigidBodyComponent>();
	}

	WeaponService* weaponService = GetOwner().GetWorld().GetService<WeaponService>();
	if (weaponService != nullptr)
	{
		weaponService->Register(this);
	}

	if (!mParticleTextureFileName.empty())
	{
		mParticleTextureId = SpEngine::Graphics::TextureManager::Get()->LoadTexture(mParticleTextureFileName);
	}

	if (!mShootSoundFileName.empty())
	{
		mShootSoundId = SpEngine::Audio::SoundEffectManager::Get()->Load(mShootSoundFileName);
	}

	// Initialize particle system
	SpEngine::Physics::ParticleSystemInfo info;
	info.textureId = mParticleTextureId;
	info.maxParticles = 50;
	info.particleLifeTime = { 0.2f, 0.4f };
	info.startScale = { Math::Vector3{0.5f}, Math::Vector3{0.5f} };
	info.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };
	info.startColor = { Colors::Orange, Colors::Yellow };
	info.endColor = { Colors::Red, Colors::Black };
	info.spawnSpeed = { 2.0f, 6.0f };
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnAngle = { -Math::Constants::Pi, Math::Constants::Pi };
	info.particlesPerEmit = { 10, 20 };
	mHitParticles.Initialize(info);
}

void WeaponComponent::Terminate()
{
	mHitParticles.Terminate();

	WeaponService* weaponService = GetOwner().GetWorld().GetService<WeaponService>();
	if (weaponService != nullptr)
	{
		weaponService->Unregister(this);
	}
}

void WeaponComponent::Update(float deltaTime)
{
	mFireCooldown -= deltaTime;
	mTraceTimer -= deltaTime;

	auto input = SpEngine::Input::InputSystem::Get();
	if (input->IsMouseDown(SpEngine::Input::MouseButton::LBUTTON) && mFireCooldown <= 0.0f)
	{
		mFireCooldown = mTimeBetweenShots;

		if (mShootSoundId != 0)
		{
			SpEngine::Audio::SoundEffectManager::Get()->Play(mShootSoundId);
		}

		if (mTransformComponent != nullptr && mCameraComponent != nullptr)
		{
			Math::Matrix4 matWorld = mTransformComponent->GetWorldTransform().GetMatrix4();
			Math::Vector3 muzzleWorld = Math::TransformCoord(mMuzzleOffset, matWorld);

			const SpEngine::Graphics::Camera& camera = mCameraComponent->GetCamera();
			Math::Vector3 lookDirection = camera.GetDirection();

			Math::Vector3 cameraPosition = camera.GetPosition();

			// ignore the player's rigid body
			const btCollisionObject* playerBody = (mRigidBodyComponent != nullptr)
				? mRigidBodyComponent->GetCollisionObject()
				: nullptr;

			auto physicsWorld = SpEngine::Physics::PhysicsWorld::Get();
			if (physicsWorld != nullptr)
			{
				SpEngine::Physics::PhysicsWorld::RaycastResult rayResult;
				if (playerBody != nullptr)
				{
					rayResult = physicsWorld->Raycast(cameraPosition, lookDirection, mRange, playerBody);
				}
				else
				{
					rayResult = physicsWorld->Raycast(cameraPosition, lookDirection, mRange);
				}

				mTraceStart = muzzleWorld;
				if (rayResult.hasHit)
				{
					mTraceEnd = rayResult.hitPoint;

					mHitParticles.SetPosition(rayResult.hitPoint);
					mHitParticles.SpawnParticles();
				}
				else
				{
					mTraceEnd = muzzleWorld + (lookDirection * mRange);
				}
				mTraceTimer = mTraceDuration;
			}
		}
	}

	if (mTraceTimer > 0.0f)
	{
		SimpleDraw::AddLine(mTraceStart, mTraceEnd, mBulletTrailColor);
	}

	mHitParticles.Update(deltaTime);
}

void WeaponComponent::DebugUI()
{
	ImGui::DragFloat3("Muzzle Offset", &mMuzzleOffset.x, 0.01f);
	ImGui::DragFloat("Range", &mRange, 1.0f, 1.0f, 1000.0f);
	ImGui::DragFloat("Time Between Shots", &mTimeBetweenShots, 0.01f, 0.01f, 10.0f);
	mHitParticles.DebugUI();
}

void WeaponComponent::Deserialize(const rapidjson::Value& value)
{
	SaveUtil::ReadVector3("MuzzleOffset", mMuzzleOffset, value);
	SaveUtil::ReadColor("TrailColor", mBulletTrailColor, value);
	SaveUtil::ReadFloat("Range", mRange, value);
	SaveUtil::ReadFloat("TimeBetweenShots", mTimeBetweenShots, value);
	SaveUtil::ReadString("ParticleTexture", mParticleTextureFileName, value);
	SaveUtil::ReadString("ShootSound", mShootSoundFileName, value);
}

void WeaponComponent::RenderParticles(SpEngine::Graphics::ParticleSystemEffect& effect)
{
	mHitParticles.Render(effect);
}