#pragma once

namespace SpEngine::Core
{
	using EventTypeId = std::size_t;

	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		virtual EventTypeId GetTypeId() const = 0;

	};
}

#define SET_EVENT_TYPE_ID(id)\
	static SpEngine::Core::EventTypeId StaticGetTypeId() { return static_cast<SpEngine::Core::EventTypeId>(id); }\
	SpEngine::Core::EventTypeId GetTypeId() const override { return StaticGetTypeId(); }