#pragma once

#include <SpEngine/Inc/TypeIds.h>

enum class CustomComponentId
{
	CustomDebugDraw = static_cast<int>(SpEngine::ComponentId::Count)
};

enum class CustomServiceId
{
	CustomDebugDrawDisplay = static_cast<int>(SpEngine::ServiceId::Count)
};