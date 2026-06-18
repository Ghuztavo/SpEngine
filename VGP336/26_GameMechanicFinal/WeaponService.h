#pragma once

#include "CustomTypeIDs.h"
#include <SpEngine/Inc/SpEngine.h>

class WeaponComponent;

class WeaponService : public SpEngine::Service {
public:
	SET_TYPE_ID(WeaponServiceId::WeaponFunctionality);

	void Register(const WeaponComponent* weaponComponent);
	void Unregister(const WeaponComponent* weaponComponent);

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

private:
	SpEngine::Graphics::ParticleSystemEffect mParticleEffect;
	using WeaponComponents = std::vector<const WeaponComponent*>;
	WeaponComponents mWeaponComponents;
};