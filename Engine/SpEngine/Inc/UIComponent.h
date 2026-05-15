#pragma once


#include "Component.h"

namespace SpEngine
{
	class UIComponent : public Component
	{
	public:
		virtual void Render() = 0;
	};
}