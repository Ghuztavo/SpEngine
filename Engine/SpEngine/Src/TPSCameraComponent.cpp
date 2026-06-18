#include "Precompiled.h"
#include "TPSCameraComponent.h"
#include "GameObject.h"	
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "SaveUtil.h"

using namespace SpEngine::Input;

void TPSCameraComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
	ASSERT(mTransformComponent != nullptr && mCameraComponent != nullptr, "TPSCameraComponent: is missing either transform or camera.");
}

void TPSCameraComponent::LateUpdate(float deltaTime)
{
	Graphics::Camera& camera = mCameraComponent->GetCamera();
	Math::Matrix4 matWorld = mTransformComponent->GetMatrix4();
	Math::Vector3 targetPosition = Math::TransformCoord(mOffset, matWorld);
	// Math::Vector3 targetDirection = Math::GetLook(matWorld);
	
	auto input = InputSystem::Get();
	mPitch += input->GetMouseMoveY() * mTurnSpeed;
	mPitch = Math::Clamp(mPitch, -1.5f, 1.5f); // clamp pitch
	
	Math::Vector3 right = Math::GetRight(matWorld);
	Math::Vector3 look = Math::GetLook(matWorld);
	Math::Matrix4 pitchRot = Math::Matrix4::RotationAxis(right, mPitch);
	Math::Vector3 targetDirection = Math::TransformNormal(look, pitchRot);
	
	targetPosition = Math::Lerp(camera.GetPosition(), targetPosition, deltaTime * mSmoothingValue);
	targetDirection = Math::Lerp(camera.GetDirection(), targetDirection, deltaTime * mSmoothingValue);
	camera.SetPosition(targetPosition);
	camera.SetDirection(targetDirection);
}

void TPSCameraComponent::DebugUI()
{
	ImGui::DragFloat3("Offset", &mOffset.x, 0.1f);
	ImGui::DragFloat("Smoothing", &mSmoothingValue, 0.1f, 0.1f, 100.0f);
	ImGui::DragFloat("TurnSpeed", &mTurnSpeed, 0.001f, 0.001f, 1.0f);
}

void TPSCameraComponent::Deserialize(const rapidjson::Value& value)
{
	SaveUtil::ReadVector3("Offset", mOffset, value);
	SaveUtil::ReadFloat("Smoothing", mSmoothingValue, value);
	SaveUtil::ReadFloat("TurnSpeed", mTurnSpeed, value);
}

void TPSCameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	// compare with original, if different, save current value
	value.AddMember("TPSCameraComponent", componentValue, doc.GetAllocator());
}