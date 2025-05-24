#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
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

// shapes values

//cube
bool filled = false;
Math::Vector3 cubeV0 = Math::Vector3::Zero;
Math::Vector3 cubeV1 = Math::Vector3::One;
Color cubeColor = Colors::White;

//sphere
int sphereSlices = 60;
int sphereRings = 60;
float sphereRadius = 1.0f;
Color sphereColor = Colors::White;
Math::Vector3 sphereOrigin = Math::Vector3::Zero;

//ground plane
int gIntValue = 10.0f;
Color gColor = Colors::White;

//ground circle
int gCircleSlices = 60;
float gCircleRadius = 10.0f;
Color gCircleColor = Colors::White;
Math::Vector3 gCircleOrigin = Math::Vector3::Zero;


enum class Shape
{
	None,
	AABB,
	Sphere,
	GroundPlane,
	GroundCircle,
	Transform
};
const char* gShapeNames[] = { "None", "AABB", "Sphere", "GroundPlane", "GroundCircle", "Transform" };

Shape gCurrentShape = Shape::None;
void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Choose a shape");

	int currentShape = (int)gCurrentShape;
	if (ImGui::Combo("Shape", &currentShape, gShapeNames, std::size(gShapeNames)))
	{
		gCurrentShape = (Shape)currentShape;
	}

	ImGui::End();

	switch (gCurrentShape)
	{
	case Shape::None: break;
	case Shape::AABB:
	{
		ImGui::Text("AABB");
		ImGui::Checkbox("Filled", &filled);
		ImGui::DragFloat3("V0", &cubeV0.x, 0.1);
		ImGui::DragFloat3("V1", &cubeV1.x, 0.1);
		ImGui::ColorEdit4("Color", &cubeColor.r);
		if (filled)
		{
			SimpleDraw::AddFilledAABB(cubeV0, cubeV1, cubeColor);
		}
		else
		{
			SimpleDraw::AddAABB(cubeV0, cubeV1, cubeColor);
		}
		break;
	}
	case Shape::Sphere:
		{

		ImGui::Text("Sphere");
		ImGui::DragInt("Slices", &sphereSlices, 1, 3, 100);
		ImGui::DragInt("Rings", &sphereRings, 1, 3, 100);
		ImGui::DragFloat("Radius", &sphereRadius, 0.1f, 0.1f, 100.0f);
		ImGui::ColorEdit4("Color", &sphereColor.r);
		ImGui::DragFloat3("Origin", &sphereOrigin.x, 0.1f);

		SimpleDraw::AddSphere(sphereSlices, sphereRings, sphereRadius, sphereColor, sphereOrigin);

		break;
		}
	case Shape::GroundPlane:
		{
		ImGui::Text("Ground Plane");
		ImGui::DragInt("Size", &gIntValue, 1, 1, 100);
		ImGui::ColorEdit4("Color", &gColor.r);
		SimpleDraw::AddGroundPlane(gIntValue, gColor);
		break;
		}
	case Shape::GroundCircle:
		{
		ImGui::Text("Ground Circle");
		ImGui::DragInt("Slices", &gCircleSlices, 1, 3, 100);
		ImGui::DragFloat("Radius", &gCircleRadius, 0.1f, 0.1f, 100.0f);
		ImGui::ColorEdit4("Color", &gCircleColor.r);
		ImGui::DragFloat3("Origin", &gCircleOrigin.x, 0.1f);
		SimpleDraw::AddGroundCircle(gCircleSlices, gCircleRadius, gCircleColor, gCircleOrigin);

			break;
		}
	case Shape::Transform:
		{
			SimpleDraw::AddTransform(Math::Matrix4::Identity);
		}
	}

	SimpleDraw::Render(mCamera);
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