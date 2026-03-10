#pragma once

#include <SpEngine/Inc/SpEngine.h>

enum class GameEventType
{
	PressSpace = 1,
	PressEnter
};

class PressSpaceEvent : public SpEngine::Core::Event
{
public:
	PressSpaceEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressSpace)
};

class PressEnterEvent : public SpEngine::Core::Event
{
public:
	PressEnterEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressEnter)
};