#pragma once

#include "CustomTypeIds.h"
#include <SpEngine/Inc/SpEngine.h>

class CustomDebugDrawComponent : public SpEngine::Component
{
public:
	SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

	void Initialize() override;
	void Terminate() override;
	void DebugUI() override;
	void Deserialize(const rapidjson::Value& value) override;

	void AddDebugDraw() const;
private:
	const SpEngine::TransformComponent* mTransformComponent = nullptr;
	SpEngine::Math::Vector3 mPosition = SpEngine::Math::Vector3::Zero;
	SpEngine::Graphics::Color mColor = SpEngine::Graphics::Colors::White;

	uint32_t mSlices = 0;
	uint32_t mRings = 0;
	float mRadius = 0;
};