#include "WeaponService.h"
#include "WeaponComponent.h"


void WeaponService::Register(const WeaponComponent* weaponComponent)
{
	auto iter = std::find(mWeaponComponents.begin(), mWeaponComponents.end(), weaponComponent);
	if (iter == mWeaponComponents.end())
	{
		mWeaponComponents.push_back(weaponComponent);
	}
}
void WeaponService::Unregister(const WeaponComponent* weaponComponent)
{
	auto iter = std::find(mWeaponComponents.begin(), mWeaponComponents.end(), weaponComponent);
	if (iter != mWeaponComponents.end())
	{
		mWeaponComponents.erase(iter);
	}
}

void WeaponService::Initialize()
{
	mParticleEffect.Initialize();
}

void WeaponService::Terminate()
{
	mParticleEffect.Terminate();
}

void WeaponService::Update(float deltaTime)
{
	for (auto weaponComponent : mWeaponComponents)
	{
		const_cast<WeaponComponent*>(weaponComponent)->Update(deltaTime);
	}
}

void WeaponService::Render()
{
	auto cameraService = GetWorld().GetService<SpEngine::CameraService>();
	if (cameraService == nullptr)
	{
		return;
	}

	mParticleEffect.SetCamera(cameraService->GetMain());

	mParticleEffect.Begin();
	for (auto weaponComponent : mWeaponComponents)
	{
		const_cast<WeaponComponent*>(weaponComponent)->RenderParticles(mParticleEffect);
	}
	mParticleEffect.End();
}