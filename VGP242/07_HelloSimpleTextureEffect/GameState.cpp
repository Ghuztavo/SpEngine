#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	// initialize gpu communication
	mSimpleTextureEffect.Initialize();

	//initialize render object
	MeshPX sphere = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mObject0.mesh.Initialize(sphere);
	mObject0.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");

	MeshPX bat = MeshBuilder::CreateOBJPX(L"../../Assets/Models/Baseball_Bat/Baseball_Bat.obj", 0.01f);
	mObject1.mesh.Initialize(bat);
	mObject1.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Models/Baseball_Bat/BaseballBat_bcolor.png", false);
	mObject1.matWorld = Math::Matrix4::Translation({ 0.0f, 1.0f, 0.0f });

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);

}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	TextureManager::Get()->ReleaseTexture(mObject0.textureId);
	TextureManager::Get()->ReleaseTexture(mObject1.textureId);
	mObject0.mesh.Terminate();
	mObject1.mesh.Terminate();
	mSimpleTextureEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	//render to render targettar
	mSimpleTextureEffect.SetCamera(mRenderTargetCamera);
	mRenderTarget.BeginRender();
		mSimpleTextureEffect.Begin();
			mSimpleTextureEffect.Render(mObject0);
			mSimpleTextureEffect.Render(mObject1);
		mSimpleTextureEffect.End();
	mRenderTarget.EndRender();

	//render to the scene
	mSimpleTextureEffect.SetCamera(mCamera);
	mSimpleTextureEffect.Begin();
		mSimpleTextureEffect.Render(mObject0);
		mSimpleTextureEffect.Render(mObject1);
	mSimpleTextureEffect.End();
}

void GameState::DebugUI()
{
	
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Choose object to render");

	ImGui::Separator();
	ImGui::Text("Render Target");
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });

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