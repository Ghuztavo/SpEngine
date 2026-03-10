#include "GameState.h"
#include "GameEvent.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
using namespace SpEngine::Physics;
using namespace SpEngine::Audio;

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mSimpleTextureEffect.Initialize();
	mSimpleTextureEffect.SetCamera(mCamera);

	MeshPX sky = MeshBuilder::CreateSkyBoxSpherePX(60, 60, 200.0f);
	mSkySphere.mesh.Initialize(sky);
	mSkySphere.textureId = TextureManager::Get()->LoadTexture(L"space.jpg");

	TextureManager* tm = TextureManager::Get();
	Mesh plane = MeshBuilder::CreatePlane(100, 100, 1.0f, true);
	mGroundObject.meshBuffer.Initialize(plane);
	mGroundObject.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
	mGroundShape.InitializeHull({ 100.0f, 0.5f, 100.0f }, { 0.0f, -0.5f, 0.0f });
	mGroundRigidBody.Initialize(mGroundObject.transform, mGroundShape);

	mParticleSystemEffect.Initialize();
	mParticleSystemEffect.SetCamera(mCamera);

}

void GameState::Terminate()
{
	mParticleSystemEffect.Terminate();
	mStandardEffect.Terminate();
	mSimpleTextureEffect.Terminate();
	mGroundRigidBody.Terminate();
	mGroundShape.Terminate();
	mGroundObject.meshBuffer.Terminate();
	mGroundObject.Terminate();
	mSkySphere.mesh.Terminate();
	
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);

}

void GameState::Render()
{
	// Render sky sphere
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(mSkySphere);
	mSimpleTextureEffect.End();


	mStandardEffect.Begin();
		mStandardEffect.Render(mGroundObject);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
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

