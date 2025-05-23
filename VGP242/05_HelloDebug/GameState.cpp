#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;

void GameState::Initialize() 
{

}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{

}

bool gCheckValue = false;
float gFloatValue = 0.0f;
Math::Vector3 gVectorValue = Math::Vector3::Zero;
Color gColor = Colors::White;
void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Hello Yall");
	ImGui::Checkbox("IsChecked", &gCheckValue);
	ImGui::DragFloat("FloatValue", &gFloatValue);
	ImGui::DragFloat3("VectorValue", &gVectorValue.x);
	ImGui::ColorEdit4("Color", &gColor.r);
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	Input::InputSystem* input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(Input::KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 1.0f;

	if (input->IsKeyDown(Input::KeyCode::W)) mCamera.Walk(moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::S)) mCamera.Walk(-moveSpeed * deltaTime);

	if (input->IsKeyDown(Input::KeyCode::A)) mCamera.Strafe(-moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::D)) mCamera.Strafe(moveSpeed * deltaTime);

	if (input->IsKeyDown(Input::KeyCode::Q)) mCamera.Rise(-moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::E)) mCamera.Rise(moveSpeed * deltaTime);

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}