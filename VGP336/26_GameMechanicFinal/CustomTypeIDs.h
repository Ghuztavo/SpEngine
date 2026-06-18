#pragma once

#include <SpEngine/Inc/TypeIds.h>

enum class WeaponComponentId
{
	Weapon = static_cast<int>(SpEngine::ComponentId::Count)
};

enum class WeaponServiceId
{
	WeaponFunctionality = static_cast<int>(SpEngine::ServiceId::Count)
};