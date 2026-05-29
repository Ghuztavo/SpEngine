#pragma once


#include "Component.h"

namespace SpEngine
{
	class UIComponent : public Component
	{
	public:
		virtual void Render() = 0;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue) override;
	};
}